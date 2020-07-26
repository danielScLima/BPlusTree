#ifndef BTREEPLUS_H
#define BTREEPLUS_H

#include "nodeOfBTreePlus.h"
#include <iostream>

/*!
 * \brief The BTreePlus class
 */
class BTreePlus
{
public:
    /*!
     * \brief BTreePlus
     * \param degree
     */
    BTreePlus(int degree = 4);
    
    ~BTreePlus();
    
    /*!
     * \brief dealockNodeOfBTrees
     * \return
     */
    void dealockNodeOfBTrees();

    /*!
     * \brief getDegree
     * \return
     */
    int getDegree();
    /*!
     * \brief entriesSize
     * \return
     */
    int entriesSize();

    /*!
     * \brief getDotFileContent
     * \return
     */
    std::string getDotFileContent();

    /*!
     * \brief getRoot
     * \return
     */
    NodeOfBTreePlus* getRoot();

    /*!
     * \brief insert
     * \param number
     * \return
     */
    bool insert(int number);
    /*!
     * \brief search
     * \param number
     * \return
     */
    NodeOfBTreePlus* search(int number);
    /*!
     * \brief remove
     * \param number
     * \return
     */
    bool remove(int number);

    /*!
     * \brief updateSequenceOfLeafPointers
     */
    void updateSequenceOfLeafPointers();
    /*!
     * \brief getSmallerNumberOfTree
     * \param rootOfSubTree
     * \return
     */
    int getSmallerNumberOfTree(NodeOfBTreePlus *rootOfSubTree);
    /*!
     * \brief removeUpstreamRemovedValue
     * \param n
     * \param removedValue
     */
    void removeUpstreamRemovedValue(NodeOfBTreePlus *n, int removedValue);

	void clearLeafChain();
private:
    NodeOfBTreePlus* root = nullptr;
    NodeOfBTreePlus* initialChildren = nullptr;

    //Is the number of cell's son
    int degree = 2;

    /*!
     * \brief fixNodeWithSmallCountOfElements
     * \param nodeToFix
     * \param removedNumber
     */
    void fixNodeWithSmallCountOfElements(NodeOfBTreePlus *nodeToFix, int removedNumber);
    /*!
     * \brief getAncestor
     * \param node
     * \param number
     * \return
     */
    NodeOfBTreePlus *getAncestor(NodeOfBTreePlus *node, int number);
};

#endif // BTREEPLUS_H
