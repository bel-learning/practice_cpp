/*  Binary search tree implementing a set of ints.
 *
 * The implementation uses double star pointers and recursion
 * to implement insert/delete operations.
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;


class CBST
{
  public:
                             CBST                          ( void );
                             CBST                          ( const CBST      & src );
                            ~CBST                          ( void );
    CBST                   & operator =                    ( const CBST      & src );
    bool                     Insert                        ( int               x );
    bool                     Delete                        ( int               x );
    bool                     IsSet                         ( int               x ) const;
    int                      Size                          ( void ) const;
    void                     Dump                          ( ostream         & os ) const;

  private:
    struct TNode
    {
                             TNode                         ( int               x );
                            ~TNode                         ( void );
      TNode                * Clone                         ( void ) const;
      void                   Print                         ( ostream         & os,
                                                             bool              first ) const;
      void                   Dump                          ( ostream         & os,
                                                             string            prefix,
                                                             string            childern ) const;
      TNode                * m_L;
      TNode                * m_R;
      int                    m_Val;
    };

    TNode                  * m_Root;
    int                      m_Count;
    bool                     addNode                       ( TNode          ** node,
                                                             int               x );
    bool                     delNode                       ( TNode          ** node,
                                                             int               x );

    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CBST      & x );
};
//-------------------------------------------------------------------------------------------------

                   CBST::CBST                              ( void )
  : m_Root ( nullptr ),
    m_Count ( 0 )
{
}
//-------------------------------------------------------------------------------------------------
                   CBST::CBST                              ( const CBST      & src )
  : m_Root ( src . m_Root ? src . m_Root -> Clone () : nullptr ),
    m_Count ( src . m_Count )
{
  /* Copy constructor, deep copy of the tree. The recursion is achieved using Clone () method in the node
   */
}
//-------------------------------------------------------------------------------------------------
                   CBST::~CBST                             ( void )
{
  /* Tree delete. The recursion is inherently implemented in the destructors.
   */
  delete m_Root;
}
//-------------------------------------------------------------------------------------------------
CBST             & CBST::operator =                        ( const CBST      & src )
{
  if ( this != &src )
  {
    /* the usual destructor/copy constructor way
     */
    delete m_Root;
    if ( src . m_Root )
      m_Root = src . m_Root -> Clone ();
    else
      m_Root = nullptr;
    m_Count = src . m_Count;
  }
  return *this;
}
//-------------------------------------------------------------------------------------------------
bool               CBST::Insert                            ( int               x )
{
  if ( addNode ( &m_Root, x ) )
  {
    /* success - increment
     */
    m_Count ++;
    return true;
  }
  return false;
}
//-------------------------------------------------------------------------------------------------
bool               CBST::Delete                            ( int               x )
 {
   if ( delNode ( &m_Root, x ) )
   {
     /* successful delete
      */
     m_Count --;
     return true;
   }
   return false;
 }
//-------------------------------------------------------------------------------------------------
bool               CBST::IsSet                             ( int               x ) const
{
  TNode * n = m_Root;

  while ( n )
  {
    if ( x == n -> m_Val )
      return true;
    if ( x < n -> m_Val )
      n = n -> m_L;
    else
      n = n -> m_R;
  }
  return false;
}
//-------------------------------------------------------------------------------------------------
int                CBST::Size                              ( void ) const
{
  return m_Count;
}
//-------------------------------------------------------------------------------------------------
bool               CBST::addNode                           ( TNode          ** node,
                                                             int               x )
{
  TNode * n;

  /*  The recursion traverses the tree and finds the position in the tree, where the new value
   * belongs to. However, we shall remember the position "where to put the address of the newly added
   * node". Thus, only the address of the pointer is insufficient, we need to use ** pointer.
   *
   * Please note this works fine for the root node to.
   */

  if ( ! *node )
  {
    /* We have reached the leaf. The branch does not continue in our direction, i.e. the inserted value
     * was not present in the tree. Create the new node and replace the nullptr with the newly created node.
     * Return success.
     */
    n = new TNode ( x );
    *node = n;
    return true;
  }

  /* Duplicate (value already present)
   */
  if ( x == (*node) -> m_Val )
    return false;
  /* Recursive traversal of the left/right subtree
   */
  if ( x < (*node) -> m_Val )
    return addNode ( &(*node) -> m_L, x );
  else
    return addNode ( &(*node) -> m_R, x );
}
//-------------------------------------------------------------------------------------------------
bool               CBST::delNode                           ( TNode          ** node,
                                                             int               x )

{
  TNode * toDel;

  /* Recursive search/delete. We have to find the node to delete, however, we shall remember
   * the place "where the delete node was referenced". We need to replace the adderss (e.g. with nullptr).
   * Thus again, double star pointer is used.
   */

  /* We reached end of the tree (leaf), however, the searched value was not present.
   */
  if ( ! *node )
    return false;
  if ( x == (*node) -> m_Val )
  {
    /* we found the value
     */
    toDel = *node;

    if ( toDel -> m_L && toDel -> m_R )
    {
      /* The node to delete (toDel) has two childs. We must find the rightmost node in the left subtree (node).
       * (Equivalently, we could find the leftmost node in the right subtree). Copy the value from the node (node)
       * to the (toDel) node. Now, we need to delete the (node) node, which has at most one child.
       */
      node = &toDel -> m_L;
      while ( (*node) -> m_R )
        node = &(*node) -> m_R;

      toDel -> m_Val = (*node) -> m_Val;
      toDel = *node;
    }

    /* The node to delete has at most one child. Replace the link to our node (toDel) with the link to the child.
     * The child may be either left or right, or there may be zero childs. If both childs are nullptr, we must replace
     * the link to (toDel) node with nullptr.
     */

    if ( toDel -> m_L )
      *node = toDel -> m_L;
    else
      *node = toDel -> m_R; /* if both childs are nullptr, nullptr will be copied here */

    /* Now cut the links to our childs. If we deleted the node without links disconnected, the recursive delete
     * would destroy our subtree (which is still in use).
     */
    toDel -> m_L = toDel -> m_R = nullptr;
    delete toDel;
    return true;
  }
  /* Traverse the tree
   */
  if ( x < (*node) -> m_Val )
    return delNode ( &(*node) -> m_L, x );
  else
    return delNode ( &(*node) -> m_R, x );
}
//-------------------------------------------------------------------------------------------------
void               CBST::Dump                              ( ostream         & os ) const
{
  if ( m_Root )
    m_Root -> Dump ( os, "", "  " );
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
                   CBST::TNode::TNode                      ( int               x )
  : m_L ( nullptr ),
    m_R ( nullptr ),
    m_Val ( x )
{
}
//-------------------------------------------------------------------------------------------------
                   CBST::TNode::~TNode                     ( void )
{
  delete m_L;
  delete m_R;
}
//-------------------------------------------------------------------------------------------------
CBST::TNode      * CBST::TNode::Clone                      ( void ) const
{
  TNode  * n;

  n = new TNode ( m_Val );
  if ( m_L )
    n -> m_L = m_L -> Clone ();
  if ( m_R )
    n -> m_R = m_R -> Clone ();
  return n;
}
//-------------------------------------------------------------------------------------------------
void               CBST::TNode::Print                      ( ostream         & os,
                                                             bool              first ) const
{
  if ( m_L )
  {
    m_L -> Print ( os, first );
    first = false;
  }
  if ( !first ) // suppress comma before the first value printed
    os << ", ";

  os << m_Val;
  if ( m_R )
    m_R -> Print ( os, false );
}
//-------------------------------------------------------------------------------------------------
void               CBST::TNode::Dump                       ( ostream         & os,
                                                             string            prefix,
                                                             string            childern ) const
{
  os << prefix << "+-" << m_Val << endl;
  if ( m_L )
    m_L -> Dump ( os, prefix + childern, m_R ? "| " : "  " );
  if ( m_R )
    m_R -> Dump ( os, prefix + childern, "  " );
}
//-------------------------------------------------------------------------------------------------
ostream         & operator <<                              ( ostream         & os,
                                                             const CBST      & x )
{
  os << "[";
  if ( x . m_Root )
    x . m_Root -> Print ( os, true );
  os << "]";
  return ( os );
}
//-------------------------------------------------------------------------------------------------
int main ( int argc, char * argv [] )
{
  CBST  a;

  a . Insert ( 100 );
  a . Insert ( 120 );
  a . Insert ( 80 );
  a . Insert ( 85 );
  a . Insert ( 90 );
  a . Insert ( 88 );
  a . Insert ( 150 );

  a . Dump ( cout );

  cout << a << endl;

  a . Delete ( 100 );

  a . Dump ( cout );
  cout << a << endl;
  a . Insert ( 100 );
  a . Insert ( 120 );
  a . Insert ( 80 );
  a . Insert ( 85 );
  a . Insert ( 90 );
  a . Insert ( 88 );

  a . Dump ( cout );

  cout << a << endl;

  a . Delete ( 100 );

  a . Dump ( cout );
  cout << a << endl;

  a . Delete ( 85 );
  a . Dump ( cout );
  a . Delete ( 120 );
  a . Delete ( 80 );
  a . Delete ( 90 );
  a . Delete ( 150 );
  a . Delete ( 88 );

  a . Dump ( cout );
  cout << a << endl;

  return 0;
}
//-------------------------------------------------------------------------------------------------