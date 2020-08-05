#include "btreeplus.h"
#include <queue>

BTreePlus::BTreePlus(int degree):
    degree(degree)
{

}

BTreePlus::~BTreePlus()
{
    dealockNodeOfBTrees();
}

void BTreePlus::dealockNodeOfBTrees()
{
    if (root != nullptr)
    {
        root->dealockSubTree();
        delete root;
        root = nullptr;
    }
}

int BTreePlus::getDegree()
{
    return this->degree;
}

int BTreePlus::entriesSize()
{
    return this->degree - 1;
}

void BTreePlus::updateSequenceOfLeafPointers()
{
    NodeOfBTreePlus * tmp = this->initialChildren;
    NodeOfBTreePlus * next = nullptr;
    while (tmp != nullptr && tmp->getNextLeaf() != nullptr)
    {
        next = tmp->getNextLeaf(); //faz backup
        tmp->resetNextLeafPointer(); //reseta ponteiro
        tmp = next; //acessa o proximo elemento
    }
    this->initialChildren = nullptr;

    NodeOfBTreePlus *last = nullptr;
    std::queue<NodeOfBTreePlus *> queue;
    if (root != nullptr)
        queue.push(root);
    while(queue.size() > 0)
    {
        NodeOfBTreePlus * node = queue.front(); //acessa o primeiro
        queue.pop(); //remove o primeiro

        std::vector<NodeOfBTreePlus *> vec = node->getVectorOfPointers();
        if (vec.size() == 0) //é leaf
        {
            if (initialChildren == nullptr)
            {
                initialChildren = node;
                last = initialChildren; //last era nullptr
                initialChildren->setNextLeaf(nullptr);
            }
            else
            {
                last->setNextLeaf(node);
                node->setNextLeaf(nullptr);
                last = node;
            }
        }
        else //não é leaf
        {
            for (auto pointer: vec)
            {
                queue.push(pointer);
            }
        }
    }
}

std::string BTreePlus::getDotFileContent()
{
    std::string msg = "digraph g { ";
    msg += "ROOT [shape=none fontcolor=blue];";
    msg += "CHILDRENS [shape=none fontcolor=blue];";
    msg += "node [shape=rect, height=0.1];";

    std::string msgLinks;

    std::queue<NodeOfBTreePlus *> queue;

    if (root != nullptr)
        queue.push(root);
    while(queue.size() > 0)
    {
        NodeOfBTreePlus * node = queue.front(); //acessa o primeiro
        queue.pop(); //remove o primeiro

        node->getDotFileRepresentation(msg);
        std::vector<NodeOfBTreePlus *> vec = node->getVectorOfPointers();
        for (auto pointer: vec)
        {
            queue.push(pointer);
            msgLinks += "\""+node->getNodeName()+"\"->\""+pointer->getNodeName()+"\";";
        }
    }

    if (root != nullptr)
    {
        std::string rootName = root->getNodeName();
        msg += "ROOT->\""+rootName+"\";";
    }
    else
    {
        msg += "ROOT->NULL;";
    }

    std::string leafNames;
    if (initialChildren != nullptr)
    {
        std::string initialChildrenName = initialChildren->getNodeName();
        msg += "CHILDRENS->\""+initialChildrenName+"\";";

        leafNames += "CHILDRENS ";
        leafNames += "\""+initialChildrenName+"\" ";

        NodeOfBTreePlus * tmp = initialChildren;
        while (tmp != nullptr && tmp->getNextLeaf() != nullptr)
        {
            msg += "\""+tmp->getNodeName()+"\"->\""+
                    tmp->getNextLeaf()->getNodeName()+"\";";

            leafNames += "\""+tmp->getNextLeaf()->getNodeName()+"\" ";

            tmp = tmp->getNextLeaf();
        }

        msg += "{rank = same; ";
        msg += leafNames;
        msg += " };";
    }
    else
    {
        msg += "CHILDRENS->NULL ;";
    }

    msg += msgLinks;

    msg += "}";

    return msg;
}

NodeOfBTreePlus *BTreePlus::getRoot()
{
    return root;
}

bool BTreePlus::insert(int number)
{
    if (root == nullptr)
    {
        root = new NodeOfBTreePlus(nullptr, this->degree);
        initialChildren = root;
    }

    clearLeafChain();//update
    bool ret = root->insert(number);

    //para atualizar leafs
    updateSequenceOfLeafPointers();

    return ret;
}

NodeOfBTreePlus * BTreePlus::search(int number)
{
    NodeOfBTreePlus * temp = root;
    bool goToNext = false;
    while (true)
    {
        const std::vector<int> vec = temp->getVectorOfEntries();
        for (int index = 0; index < vec.size(); ++index)
        {
            if (number == vec[index] && temp->isALeafNode())
                return temp;
            else if (number == vec[index]) //não é leaf
            {
                if (index+1 < temp->getVectorOfPointers().size())
                {
                    temp = temp->pointerByIndex(index+1);
                    goToNext = true;
                    break;
                }
            }
            else if (number < vec[index])
            {
                if (index < temp->getVectorOfPointers().size())
                {
                    temp = temp->pointerByIndex(index);
                    goToNext = true;
                    break;
                }
                else
                    return nullptr;
            }
        }
        if (goToNext == false)
        {
            //Verificar no último ponteiro
            if (temp->getVectorOfPointers().size() > 1) //deve ter pelo menos 2 ponteiros
            {
                temp = temp->getVectorOfPointers().back();
            }
            else
                return nullptr;
        }
        else
        {
            goToNext = false;
        }
    }
    return nullptr;
}

bool BTreePlus::remove(int number)
{
    int maxSize = degree-1;
    NodeOfBTreePlus * n = search(number);
    if (n == nullptr)
        return false;
    else
    {
        //Verifica se o nó é uma folha
        if (n->getVectorOfPointers().size() == 0)
        {
            //caso 1: Verifica se tem mais elementos do que a metade
            //do tamanho máximo (antes de remover)
            int size = n->getVectorOfEntries().size();
            if (size > maxSize/2)
            {
                //A função abaixo remove posições internas do vetor tmb
                n->removeEntryOfVectorOfEntries(number);
                removeUpstreamRemovedValue(n, number);
            }
            else
            {
                //remove
                n->removeEntryOfVectorOfEntries(number);

                //verifica como resolver, se for preciso
                size = n->getVectorOfEntries().size();
				if (size < maxSize / 2)
				{
					clearLeafChain();
					fixNodeWithSmallCountOfElements(n, number);
				}
            }
        }
        else //Não é uma folha
        {
            //caso 2: troca com o seu antecessor
            //isso implica em remover do antecessor um numero (o maior)
            //Pode ter que resolver o problema do antecessor
            NodeOfBTreePlus * ancestor = getAncestor(n, number);
            int numberAncestor = ancestor->getVectorOfEntries().back();
            ancestor->removeEntryOfVectorOfEntries(numberAncestor);

            //ancestor é folha, não tem ponteiros filhos

            //Em n, troca o número number por numberAncestor
            n->replaceEntryInVectorOfEntries(number, numberAncestor);

            //Verifica se tem que resolver e resolve, se necessário
            int size = ancestor->getVectorOfEntries().size();
			if (size < maxSize / 2)
			{
				clearLeafChain();
				fixNodeWithSmallCountOfElements(ancestor, number);
			}
        }

        //para atualizar leafs
        updateSequenceOfLeafPointers();

        return true;
    }
}

void BTreePlus::clearLeafChain()
{
	NodeOfBTreePlus * tmp = this->initialChildren;
	NodeOfBTreePlus * next = nullptr;
	while (tmp != nullptr && tmp->getNextLeaf() != nullptr)
	{
		next = tmp->getNextLeaf(); //faz backup
		tmp->resetNextLeafPointer(); //reseta ponteiro
		tmp = next; //acessa o proximo elemento
	}
	this->initialChildren = nullptr;
}

void BTreePlus::removeUpstreamRemovedValue(NodeOfBTreePlus * n, int removedValue)
{
    //deve checar se tem o numero acima
    NodeOfBTreePlus *father = n->getFather();
    bool goToNext = true;
    while (n != nullptr && father != nullptr && goToNext)
    {
        std::vector<int> vecent = father->getVectorOfEntries();
        for (int i = 0; i< vecent.size(); ++i)
        {
            if (vecent[i] == removedValue)
            {
                int smaller = getSmallerNumberOfTree
                (
                    father->getVectorOfPointers().at(i+1)
                );

                father->setEntryByIndex(i, smaller);

                goToNext = false;
                break;
            }
        }
        if (goToNext)
        {
            n = father;
            father = n->getFather();
        }
    }
}

NodeOfBTreePlus * BTreePlus::getAncestor(NodeOfBTreePlus * node, int number)
{
    int pos = 0;
    std::vector<int> vecOfEntries = node->getVectorOfEntries();
    for (int index = 0; index < vecOfEntries.size(); ++index)
    {
        if (vecOfEntries[index] == number)
            pos = index;
    }

    NodeOfBTreePlus * nodePointer = node->getVectorOfPointers().at(pos);
    while (nodePointer->getVectorOfPointers().size() > 0)
    {
        nodePointer = nodePointer->getVectorOfPointers().back();
    }
    return nodePointer;
}

void BTreePlus::fixNodeWithSmallCountOfElements(NodeOfBTreePlus *nodeToFix, int removedNumber)
{
    int maxSize = degree-1;
    if (nodeToFix->getVectorOfEntries().size() >= maxSize/2)
        return; //está resolvido

    if (nodeToFix == root) //o root pode ter menos elementos
    {
        if (nodeToFix->getVectorOfEntries().size() == 0)
        {
            //juntar filhos
            //Tem 2 filhos restantes (pelas propriedades da remoção)
            std::vector<NodeOfBTreePlus *> pointers = nodeToFix->getVectorOfPointers();
            //NodeOfBTreePlus * tmp = pointers[0];
            for (auto tmp: pointers)
            {
                while (tmp->getVectorOfEntries().size() > 0)
                {
                    int first = tmp->removeFirstPositionOfVectorOfEntries();
                    if (first != removedNumber)
                        nodeToFix->insertInLastPositionOfVectorOfEntries(first);
                }
            }
            while (nodeToFix->getVectorOfPointers().size() > 0)
            {                
                //change 1
                NodeOfBTreePlus * n = nodeToFix->removeLastPointer();
            }

            std::vector<NodeOfBTreePlus *> childrens;
            for (auto tmp: pointers)
            {
                while (tmp->getVectorOfPointers().size() > 0)
                {
                    NodeOfBTreePlus * firstP = tmp->removeFirstPointer();
                    childrens.push_back(firstP);
                }
            }
            for (auto ptr: childrens)
            {
                nodeToFix->insertInLastPositionOfVectorOfPointers(ptr);
                ptr->setFather(nodeToFix);
            }
			//aqui sim, deletar
			for (auto tmp : pointers)
			{
				delete tmp;
			}
        }

        return;
    }

    int indexFound = 0;
    //Verifica se pode obter de algum irmão, um valor excedente
    NodeOfBTreePlus * father = nodeToFix->getFather();
    std::vector<NodeOfBTreePlus *> pointers = father->getVectorOfPointers();
    for (int index = 0; index < pointers.size(); ++index)
    {
        if (nodeToFix == pointers[index])
        {
            indexFound = index;
            //Verifica o irmão esquerdo, se tiver
            if (index >= 1)
            {
                NodeOfBTreePlus * leftBrother = pointers[index-1];
                if (leftBrother->getVectorOfEntries().size() > maxSize/2)
                {
                    //faz e retorna
                    //Este faz não esta considerando ponteiros filhos
                    int last = leftBrother->removeLastPositionOfVectorOfEntries();
                    int top = father->entryByIndex(index-1); // obtem o numero de cima
                    father->setEntryByIndex(index-1, last);
                    //insere número de cima na posição zero de nodeToFix
                    nodeToFix->insertInFirstPositionOfVectorOfEntries(top);

                    if (top == removedNumber)
                    {
                        //Obtem o novo top
                        int top = father->entryByIndex(index-1);
                        nodeToFix->setEntryByIndex(0, top);
                    }

                    //Verificar se tem irmão da direita
                    //Verifica se o irmao tem ponteiro. Se tiver, remove o ultimo
                    if (leftBrother->getVectorOfPointers().size() > 0 &&
                            (index+1) < pointers.size())
                    {
                        NodeOfBTreePlus * last = leftBrother->removeLastPointer();
                        NodeOfBTreePlus * rightBrother = pointers[index+1];
                        rightBrother->insertInFirstPositionOfVectorOfPointers(last);
                        last->setFather(rightBrother);
                    }

                    return;
                }
            }

            if (index < pointers.size()-1)
            {
                NodeOfBTreePlus * rightBrother = pointers[index+1];
                if (rightBrother->getVectorOfEntries().size() > maxSize/2)
                {
                    //faz e retorna
                    //Este faz não esta considerando ponteiros filhos
                    int first = rightBrother->removeFirstPositionOfVectorOfEntries();
                    int top = father->entryByIndex(index); // obtem o numero de cima
                    father->setEntryByIndex(index, first);
                    nodeToFix->insertInLastPositionOfVectorOfEntries(top);

                    if (first == top)
                    {
                        int smaller = getSmallerNumberOfTree(rightBrother);
                        father->setEntryByIndex(index, smaller);
                    }

                    //Verificar se tem irmão da direita
                    //Verifica se o irmao tem ponteiro. Se tiver, remove o ultimo
                    if (rightBrother->getVectorOfPointers().size() > 0 &&
                            (index-1) >= 0)
                    {
                        NodeOfBTreePlus * first = rightBrother->removeFirstPointer();
                        NodeOfBTreePlus * leftBrother = pointers[index-1];
                        leftBrother->insertInFirstPositionOfVectorOfPointers(first);
                        first->setFather(leftBrother);
                    }

                    return;
                }
            }

            break; //tratar fora do laço
        }
    }

    //Se chegou aqui, é porque nem o esquerdo, nem o direito, tinham
    //possibilidade de ajudar
    //Junta com o irmão esquerdo (podia ser o direito também)
    if (indexFound > 0) //tem irmão esquerdo
    {
        //Juntar com o esquerdo
        //deletar o esquerdo
        //Remover primeira entrada do pai (valor)
        int top = father->entryByIndex(indexFound-1);
        father->removeByIndexInVectorOfEntries(indexFound-1);

        if (top != removedNumber)
        {
            if (nodeToFix->getVectorOfEntries().size() == 0)
                nodeToFix->insertInFirstPositionOfVectorOfEntries(top);
            else
            {
                int first = nodeToFix->getVectorOfEntries().at(0);
                if (first != top)
                    nodeToFix->insertInFirstPositionOfVectorOfEntries(top);
            }
        }

        NodeOfBTreePlus * leftBrother = pointers[indexFound-1];
        while(leftBrother->getVectorOfEntries().size() > 0)
        {
            int last = leftBrother->removeLastPositionOfVectorOfEntries();
            nodeToFix->insertInFirstPositionOfVectorOfEntries(last);
        }
        while(leftBrother->getVectorOfPointers().size() > 0)
        {
            NodeOfBTreePlus * pointer = leftBrother->removeLastPointer();
            nodeToFix->insertInFirstPositionOfVectorOfPointers(pointer);
            pointer->setFather(nodeToFix);
        }

        //remover o left do pai
        father->removePointerOfVectorOfPointers(indexFound-1);
        //Deve propagar checagens para cima
        //mas agora não
        
        //change 2
        delete leftBrother;
    }
    else if (indexFound < pointers.size()-1) //tem irmão direito
    {
        //Juntar com o direito
        int top = father->entryByIndex(indexFound);
        father->removeByIndexInVectorOfEntries(indexFound);
        nodeToFix->insertInLastPositionOfVectorOfEntries(top);
        NodeOfBTreePlus * rightBrother = pointers[indexFound+1];
        while(rightBrother->getVectorOfEntries().size() > 0)
        {
            int first = rightBrother->removeFirstPositionOfVectorOfEntries();
            if (first != top) //diff
                nodeToFix->insertInLastPositionOfVectorOfEntries(first);
        }
        while(rightBrother->getVectorOfPointers().size() > 0)
        {
            NodeOfBTreePlus * pointer = rightBrother->removeFirstPointer();
            nodeToFix->insertInLastPositionOfVectorOfPointers(pointer);
            pointer->setFather(nodeToFix);
        }

        //remover o right do pai
        father->removePointerOfVectorOfPointers(indexFound+1);
        //Deve propagar checagens para cima
        //mas agora não

        //change 3
        delete rightBrother;
    }

    //antes de indicar resolver nos avós, remove referencias, pois a alteração
    //de merge já foi feita

    removeUpstreamRemovedValue(nodeToFix, removedNumber);

    if (father->getVectorOfEntries().size() < maxSize/2)
        fixNodeWithSmallCountOfElements(father, removedNumber);
}

int BTreePlus::getSmallerNumberOfTree(NodeOfBTreePlus * rootOfSubTree)
{
    NodeOfBTreePlus * tmp = rootOfSubTree;
    while (tmp->getVectorOfPointers().size() > 0)
    {
        tmp = tmp->getVectorOfPointers().front();
    }
    return tmp->getVectorOfEntries().front();
}
