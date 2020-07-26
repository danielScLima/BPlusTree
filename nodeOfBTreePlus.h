#ifndef NODE_OF_BTREE_PLUS_H
#define NODE_OF_BTREE_PLUS_H

#include <vector>
#include <iostream>

class BTreePlus;

/*!
 * \brief The NodeOfBTreePlus class
 */
class NodeOfBTreePlus
{
public:
    /*!
     * \brief NodeOfBTreePlus
     * \param father
     * \param degree
     */
    NodeOfBTreePlus(NodeOfBTreePlus* father, int degree);
    
    /*!
     * \brief NodeOfBTreePlus
     */
    void dealockSubTree();

    /*!
     * \brief getFather
     * \return
     */
    NodeOfBTreePlus* getFather();
    /*!
     * \brief setFather
     * \param father
     */
    void setFather(NodeOfBTreePlus* father);

    /*!
     * \brief entryByIndex
     * \param index
     * \return
     */
    int entryByIndex(int index);
    /*!
     * \brief setEntryByIndex
     * \param index
     * \param number
     */
    void setEntryByIndex(int index, int number);

    /*!
     * \brief pointerByIndex
     * \param index
     * \return
     */
    NodeOfBTreePlus* pointerByIndex(int index);

    /*!
     * \brief setDegree
     * \param degree
     */
    void setDegree(int degree);

    /*!
     * \brief getDotFileRepresentation
     * \param msg
     */
    void getDotFileRepresentation(std::string &msg);

    /*!
     * \brief getVectorOfPointers
     * \return
     */
    std::vector<NodeOfBTreePlus*> getVectorOfPointers();

    /*!
     * \brief getNodeName
     * \return
     */
    const std::string getNodeName();

    /*!
     * \brief insert
     * \param number
     * \return
     */
    bool insert(int number);

    /*!
     * \brief reconfigureLeafChainInLeftBrother
     * \param father
     * \param node
     * \param newNodeOfSequence
     */
	void reconfigureLeafChainInLeftBrother
	(
		NodeOfBTreePlus* father,
        NodeOfBTreePlus* node,
		NodeOfBTreePlus* newNodeOfSequence
	);

    /*!
     * \brief receiveNodeToInsertFromDownside
     * \param originalNode
     * \param left
     * \param right
     * \param numberToInsert
     */
    void receiveNodeToInsertFromDownside
    (
        NodeOfBTreePlus *originalNode,
        NodeOfBTreePlus *left,
        NodeOfBTreePlus *right,
        int numberToInsert
    );

    /*!
     * \brief getVectorOfEntries
     * \return
     */
    std::vector<int> getVectorOfEntries();
    /*!
     * \brief removeEntryOfVectorOfEntries
     * \param number
     */
    void removeEntryOfVectorOfEntries(int number);

    /*!
     * \brief removeLastPositionOfVectorOfEntries
     * \return
     */
    int removeLastPositionOfVectorOfEntries();
    /*!
     * \brief removeFirstPositionOfVectorOfEntries
     * \return
     */
    int removeFirstPositionOfVectorOfEntries();
    /*!
     * \brief insertInFirstPositionOfVectorOfEntries
     * \param number
     */
    void insertInFirstPositionOfVectorOfEntries(int number);
    /*!
     * \brief insertInLastPositionOfVectorOfEntries
     * \param number
     */
    void insertInLastPositionOfVectorOfEntries(int number);
    /*!
     * \brief replaceEntryInVectorOfEntries
     * \param number
     * \param ancestorNumber
     */
    void replaceEntryInVectorOfEntries(int number, int ancestorNumber);
    /*!
     * \brief removeLastPointer
     * \return
     */
    NodeOfBTreePlus *removeLastPointer();
    /*!
     * \brief removeFirstPointer
     * \return
     */
    NodeOfBTreePlus *removeFirstPointer();
    /*!
     * \brief insertInFirstPositionOfVectorOfPointers
     * \param pointer
     */
    void insertInFirstPositionOfVectorOfPointers(NodeOfBTreePlus *pointer);
    /*!
     * \brief insertInLastPositionOfVectorOfPointers
     * \param pointer
     */
    void insertInLastPositionOfVectorOfPointers(NodeOfBTreePlus *pointer);
    /*!
     * \brief removeByIndexInVectorOfEntries
     * \param index
     */
    void removeByIndexInVectorOfEntries(int index);
    /*!
     * \brief removePointerOfVectorOfPointers
     * \param index
     */
    void removePointerOfVectorOfPointers(int index);

    /*!
     * \brief resetNextLeafPointer
     */
    void resetNextLeafPointer();
    /*!
     * \brief getNextLeaf
     * \return
     */
    NodeOfBTreePlus *getNextLeaf();
    /*!
     * \brief setNextLeaf
     * \param next
     */
    void setNextLeaf(NodeOfBTreePlus *next);
    /*!
     * \brief isALeafNode
     * \return
     */
    bool isALeafNode();
private:
    int degree = 2;
    NodeOfBTreePlus* father = nullptr;
    NodeOfBTreePlus *nextLeaf = nullptr;

    std::vector<int> vectorOfEntries;
    std::vector<NodeOfBTreePlus*> vectorOfPointers;
};

#endif // NODE_OF_BTREE_PLUS_H
