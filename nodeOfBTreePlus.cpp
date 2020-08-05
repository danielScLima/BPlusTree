#include "nodeOfBTreePlus.h"
#include <algorithm>
#include <string>

NodeOfBTreePlus::NodeOfBTreePlus(NodeOfBTreePlus* father, int degree):
    degree(degree), father(father)
{

}

void NodeOfBTreePlus::dealockSubTree()
{
    NodeOfBTreePlus* child = nullptr;
    for (int index = 0; index < vectorOfPointers.size(); ++index)
    {
        child = vectorOfPointers.at(index);
        if (child != nullptr)
        {
            child->dealockSubTree();
            delete child;
            vectorOfPointers[index] = nullptr;
        }
    }

    vectorOfPointers.clear();
    vectorOfEntries.clear();
}

NodeOfBTreePlus *NodeOfBTreePlus::getFather()
{
    return this->father;
}

void NodeOfBTreePlus::setFather(NodeOfBTreePlus *father)
{
    this->father = father;
}

void NodeOfBTreePlus::removeEntryOfVectorOfEntries(int number)
{
    std::vector<int>::iterator position = std::find
    (
        vectorOfEntries.begin(),
        vectorOfEntries.end(),
        number
    );
    if (position != vectorOfEntries.end())
        vectorOfEntries.erase(position);
}

void NodeOfBTreePlus::removePointerOfVectorOfPointers(int index)
{
    vectorOfPointers.erase(vectorOfPointers.begin()+index);
}

void NodeOfBTreePlus::resetNextLeafPointer()
{
    this->nextLeaf = nullptr;
}

NodeOfBTreePlus *NodeOfBTreePlus::getNextLeaf()
{
    return this->nextLeaf;
}

void NodeOfBTreePlus::setNextLeaf(NodeOfBTreePlus *next)
{
    this->nextLeaf = next;
}

bool NodeOfBTreePlus::isALeafNode()
{
    return this->vectorOfPointers.size() == 0;
}

int NodeOfBTreePlus::removeLastPositionOfVectorOfEntries()
{
    int last = vectorOfEntries.back();
    vectorOfEntries.pop_back();
    return last;
}

int NodeOfBTreePlus::removeFirstPositionOfVectorOfEntries()
{
    int first = vectorOfEntries.front();
    vectorOfEntries.erase(vectorOfEntries.begin());
    return first;
}

NodeOfBTreePlus* NodeOfBTreePlus::removeLastPointer()
{
    NodeOfBTreePlus* last = vectorOfPointers.back();
    vectorOfPointers.pop_back();
    return last;
}

NodeOfBTreePlus* NodeOfBTreePlus::removeFirstPointer()
{
    NodeOfBTreePlus* first = vectorOfPointers.front();
    vectorOfPointers.erase(vectorOfPointers.begin());
    return first;
}

void NodeOfBTreePlus::insertInFirstPositionOfVectorOfEntries(int number)
{
    vectorOfEntries.insert(vectorOfEntries.begin(), number);
}

void NodeOfBTreePlus::insertInFirstPositionOfVectorOfPointers(NodeOfBTreePlus* pointer)
{
    vectorOfPointers.insert(vectorOfPointers.begin(), pointer);
}

void NodeOfBTreePlus::insertInLastPositionOfVectorOfEntries(int number)
{
    vectorOfEntries.push_back(number);
}

void NodeOfBTreePlus::insertInLastPositionOfVectorOfPointers(NodeOfBTreePlus* pointer)
{
    vectorOfPointers.push_back(pointer);
}

void NodeOfBTreePlus::replaceEntryInVectorOfEntries(int number, int ancestorNumber)
{
    std::replace
    (
        vectorOfEntries.begin(),
        vectorOfEntries.end(),
        number,
        ancestorNumber
    );
}

std::vector<int> NodeOfBTreePlus::getVectorOfEntries()
{
    return this->vectorOfEntries;
}

void NodeOfBTreePlus::removeByIndexInVectorOfEntries(int index)
{
    if (index < vectorOfEntries.size())
        vectorOfEntries.erase(vectorOfEntries.begin()+index);
    else
        throw std::invalid_argument("Invalid index");
}

int NodeOfBTreePlus::entryByIndex(int index)
{
    if (index < vectorOfEntries.size())
        return vectorOfEntries.at(index);
    else
        throw std::invalid_argument("Invalid index");
}

void NodeOfBTreePlus::setEntryByIndex(int index, int number)
{
    if (index < vectorOfEntries.size())
        vectorOfEntries[index] = number;
}

NodeOfBTreePlus *NodeOfBTreePlus::pointerByIndex(int index)
{
    if (index < this->vectorOfPointers.size())
        return this->vectorOfPointers.at(index);
    else
        throw std::invalid_argument("Invalid index");
}

void NodeOfBTreePlus::setDegree(int degree)
{
    this->degree = degree;
}

const std::string NodeOfBTreePlus::getNodeName()
{
    std::string name;
    int maxEntries = this->degree - 1;
    for (int index = 0; index < maxEntries; ++index)
    {
        if (index < vectorOfEntries.size())
            name += std::to_string(vectorOfEntries[index]);
        else
            name += "-";

        if (index < maxEntries-1)
            name += "_";
    }
    return name;
}

void NodeOfBTreePlus::reconfigureLeafChainInLeftBrother
(
	NodeOfBTreePlus* father,
    NodeOfBTreePlus* node,
	NodeOfBTreePlus* newNodeOfSequence
)
{
	if (father == nullptr)
		return;
	for (int index = 0; index < father->getVectorOfPointers().size(); ++index)
	{
		NodeOfBTreePlus* current = father->getVectorOfPointers().at(index);
		if (current == node)
		{
			if (index > 0) //tem irmão esquerdo
			{
				NodeOfBTreePlus* leftOfCurrent = father->getVectorOfPointers().at(index-1);
				leftOfCurrent->setNextLeaf(newNodeOfSequence);
			}
		}
	}
}

bool NodeOfBTreePlus::insert(int number)
{
    //apenas para ter algo
    bool numberInserted = false;
    bool stopProcessHere = false;

    int maxEntries = this->degree - 1;
    for (int index = 0; index < this->vectorOfEntries.size(); ++index)
    {
        if (number == vectorOfEntries[index])
            return false;

        if (number < vectorOfEntries[index])
        {
            //Existe um ponteiro com posição: index?
            //Se sim, manda inserir neste ponteiro
            if (index < vectorOfPointers.size())
            {
                bool ret = vectorOfPointers[index]->insert(number);
                if (!ret)
                    return false;
                stopProcessHere = true;
                break;
            }
            else //Se não, insere no nó atual, deslocando
            {
                //Verifica se tem espaço nesse nó
                //Deve verificar a quantidade de entradas
                //se é menor que a máxima
                if (this->vectorOfEntries.size() < maxEntries)
                {
                    //A chamada abaixo já desloca as entradas
                    vectorOfEntries.insert(vectorOfEntries.begin()+index, number);
                    numberInserted = true;
                    break;
                }
            }
        }
    }

    if (numberInserted || stopProcessHere)
    {
        //não tem mais nada a fazer
    }
    else
    {
        if (this->vectorOfEntries.size() < maxEntries)
        {
            //Insere depois do último
            //Se esse ponteirofor diferente de null, insere lá
            int sizeEntries = this->vectorOfEntries.size();
            if (sizeEntries < vectorOfPointers.size() &&
                    vectorOfPointers.back() != nullptr)
            {
                bool ret = vectorOfPointers.back()->insert(number);
                if (!ret)
                    return false;
            }
            else
            {
                this->vectorOfEntries.push_back(number);
                numberInserted = true;
            }
        }
        else //tem q inserir e quebrar
        {
            if
            (
                vectorOfPointers.size() > vectorOfEntries.size() &&
                vectorOfPointers.back() != nullptr
            )
            {
                bool ret = vectorOfPointers.back()->insert(number);
                if (!ret)
                    return false;
            }
            else
            {

                //Neste ponto o novo número é maior que todos os numeros existentes
                //Deve verificar se o pai é nulo ou não
                //se for nulo (é o root), faz o codigo abaixo
                if (father == nullptr)
                {
                    numberInserted = false;
                    int pos = -1;
                    for (int index = 0; index < this->vectorOfEntries.size(); ++index)
                    {
                        if (number == vectorOfEntries[index])
                            return false;
                        if (number < vectorOfEntries[index])
                        {
                            vectorOfEntries.insert(vectorOfEntries.begin()+index, number);
                            numberInserted = true;
                            pos = index;
                            break;
                        }
                    }
                    if (numberInserted == false)
                        vectorOfEntries.push_back(number);

                    int middle = maxEntries / 2;
                    //Neste ponto existe uma entrada a mais
                    //end of new

                    //Deve quebrar o nó, pois a quantidade de elementos lotou
                    //Deve saber qual a ordem de inserção
                    //int middle = maxEntries / 2;
                    NodeOfBTreePlus* left = new NodeOfBTreePlus(this, this->degree);
                    NodeOfBTreePlus* right = new NodeOfBTreePlus(this, this->degree);
                    for (int index = 0; index < middle; ++index)
                    {
                        left->insert(vectorOfEntries[index]);
                    }
					if (this->isALeafNode())
					{
						right->insert(vectorOfEntries[middle]);

						//todo: clear link of left brother to him
						reconfigureLeafChainInLeftBrother
						(
							this->father,
							this, //
							left
						);
						left->setNextLeaf(right);
						right->setNextLeaf(getNextLeaf());
					}
                    for (int index = middle+1; index < vectorOfEntries.size(); ++index)
                    {
                        right->insert(vectorOfEntries[index]);
                    }
                    int backupNumber = vectorOfEntries[middle];
                    vectorOfEntries.clear();
                    vectorOfPointers.clear();
                    vectorOfEntries.push_back(backupNumber);
                    vectorOfPointers.push_back(left);
                    vectorOfPointers.push_back(right);
                }
                else
                {
                    //Deve dividir e enviar para o pai receber
                    //pai recebe:
                    //O ponteiro antigo (ele não deve ser mais usado)
                    //Os dois novos ponteiros
                    //O valor que deve ser recebido

                    //Deve quebrar o nó, pois a quantidade de elementos lotou
                    //Deve saber qual a ordem de inserção
                    numberInserted = false;
                    int pos = -1;
                    for (int index = 0; index < this->vectorOfEntries.size(); ++index)
                    {
                        if (number == vectorOfEntries[index])
                            return false;
                        if (number < vectorOfEntries[index])
                        {
                            vectorOfEntries.insert(vectorOfEntries.begin()+index, number);
                            numberInserted = true;
                            pos = index;
                            break;
                        }
                    }
                    if (numberInserted == false)
                        vectorOfEntries.push_back(number);

                    int middle = maxEntries / 2;

                    NodeOfBTreePlus* left = new NodeOfBTreePlus(this, this->degree);
                    NodeOfBTreePlus* right = new NodeOfBTreePlus(this, this->degree);
                    for (int index = 0; index < middle; ++index)
                    {
                        left->insert(vectorOfEntries[index]);
                    }
					if (this->isALeafNode())
					{
						right->insert(vectorOfEntries[middle]);

						//todo: clear link of left brother to him
						reconfigureLeafChainInLeftBrother
						(
							this->father,
							this, //
							left
						);
						left->setNextLeaf(right);
						right->setNextLeaf(getNextLeaf());
					}
                    for (int index = middle+1; index < vectorOfEntries.size(); ++index)
                    {
                        right->insert(vectorOfEntries[index]);
                    }
                    int backupNumber = vectorOfEntries[middle];

                    vectorOfEntries.clear();
                    vectorOfPointers.clear();

                    //faz chamada para o pai receber tudo
                    father->receiveNodeToInsertFromDownside
                    (
                        this,
                        left,
                        right,
                        backupNumber
                    );
                }
            }
        }
    }

    return true;
}

void NodeOfBTreePlus::receiveNodeToInsertFromDownside
(
    NodeOfBTreePlus* originalNode,
    NodeOfBTreePlus* left,
    NodeOfBTreePlus* right,
    int numberToInsert
)
{
    left->father = this;
    right->father = this;

    int maxEntries = this->degree - 1;

    //caso no qual o nó não está full, assim ele não se divide
    int insertionPosition = 0;

    for (int index = 0; index < this->vectorOfPointers.size(); ++index)
    {
        if (vectorOfPointers[index] == originalNode)
        {
            insertionPosition = index;
        }
    }

    if (insertionPosition == vectorOfEntries.size())
    {
        vectorOfEntries.push_back(numberToInsert);
    }
    else //é menor
    {
        vectorOfEntries.insert
        (
            vectorOfEntries.begin()+insertionPosition,
            numberToInsert
        );
    }

    if (vectorOfPointers.size() == 0)
    {
        vectorOfPointers.push_back(left);
        vectorOfPointers.push_back(right);
    }
    else
    {
    	//verifiy the cleanup
    	if (vectorOfPointers.size() > insertionPosition)
    	{
    		if (vectorOfPointers.at(insertionPosition) != left && vectorOfPointers.at(insertionPosition) != right)
    			delete vectorOfPointers.at(insertionPosition);
    	}
    
        vectorOfPointers[insertionPosition] = left;
        vectorOfPointers.insert(vectorOfPointers.begin()+insertionPosition+1, right);
    }

    if (vectorOfEntries.size() > maxEntries)
    {
        if (father == nullptr)
        {
            int middle = vectorOfEntries.size() / 2;

            NodeOfBTreePlus* leftMe = new NodeOfBTreePlus(this, this->degree);
            NodeOfBTreePlus* rightMe = new NodeOfBTreePlus(this, this->degree);
            for (int index = 0; index < middle; ++index)
            {
                leftMe->insert(vectorOfEntries[index]);
            }
            for (int index = middle+1; index < vectorOfEntries.size(); ++index)
            {
                rightMe->insert(vectorOfEntries[index]);
            }
            int backupNumber = vectorOfEntries[middle];

            for (int index = 0; index <= middle; ++index)
            {
                vectorOfPointers[index]->father = leftMe;
                leftMe->vectorOfPointers.push_back(vectorOfPointers[index]);
            }
            for (int index = middle+1; index < vectorOfPointers.size(); ++index)
            {
                vectorOfPointers[index]->father = rightMe;
                rightMe->vectorOfPointers.push_back(vectorOfPointers[index]);
            }

            vectorOfEntries.clear();
            vectorOfPointers.clear();
            vectorOfEntries.push_back(backupNumber);
            vectorOfPointers.push_back(leftMe);
            vectorOfPointers.push_back(rightMe);
        } //fim de root
        else //quando não é root o full
        {
            int middle = vectorOfEntries.size() / 2;

            NodeOfBTreePlus* leftMe = new NodeOfBTreePlus(this, this->degree);
            NodeOfBTreePlus* rightMe = new NodeOfBTreePlus(this, this->degree);
            for (int index = 0; index < middle; ++index)
            {
                leftMe->insert(vectorOfEntries[index]);
            }
            for (int index = middle+1; index < vectorOfEntries.size(); ++index)
            {
                rightMe->insert(vectorOfEntries[index]);
            }
            int backupNumber = vectorOfEntries[middle];

            for (int index = 0; index <= middle; ++index)
            {
                vectorOfPointers[index]->father = leftMe;
                leftMe->vectorOfPointers.push_back(vectorOfPointers[index]);
            }
            for (int index = middle+1; index < vectorOfPointers.size(); ++index)
            {
                vectorOfPointers[index]->father = rightMe;
                rightMe->vectorOfPointers.push_back(vectorOfPointers[index]);
            }

            vectorOfEntries.clear();
            vectorOfPointers.clear();

            //faz chamada para o pai receber tudo
            father->receiveNodeToInsertFromDownside
            (
                this,
                leftMe,
                rightMe,
                backupNumber
            );
        }
    }
}

void NodeOfBTreePlus::getDotFileRepresentation(std::string & msg)
{
    int maxEntries = this->degree - 1;
    std::string name = getNodeName();

    msg += "\""+name+"\" [label = \"";

    //Inserir as partes internas
    for (int index = 0; index < maxEntries; ++index)
    {
        if (index < vectorOfEntries.size())
        {
            msg += " "+std::to_string(vectorOfEntries[index])+" ";
        }
        else
        {
            msg += " - ";
        }

        if (index < maxEntries-1)
            msg += "| ";
    }

    msg += "\"];";
}

std::vector<NodeOfBTreePlus *> NodeOfBTreePlus::getVectorOfPointers()
{
    return this->vectorOfPointers;
}
