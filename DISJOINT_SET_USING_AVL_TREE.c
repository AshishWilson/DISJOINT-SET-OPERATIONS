// C program to insert a node in AVL tree
#include<stdio.h>
#include<stdlib.h>

// An AVL tree node
typedef struct Node{
	int key;
	struct Node *left;
	struct Node *right;
	struct Node *root;
	int height;
	int S_height;
}TNode;

int max(int a, int b);
int height(TNode *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

// A utility function to get maximum of two integers
int max(int a, int b)
{
	return (a > b)? a : b;
}

TNode* newNode(int key)
{
	TNode* node = (TNode*)malloc(sizeof(TNode));
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;// new node is initially added at leaf
	node->root = node;
	node->S_height = 1;
	return(node);
}

TNode *rightRotate(TNode *y)
{
	TNode *x = y->left;
	TNode *T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height(y->left), height(y->right))+1;
	x->height = max(height(x->left), height(x->right))+1;

	// Return new root
	return x;
}

TNode *leftRotate(TNode *x)
{
	TNode *y = x->right;
	TNode *T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height(x->left), height(x->right))+1;
	y->height = max(height(y->left), height(y->right))+1;

	// Return new root
	return y;
}

int getBalance(TNode *N)
{
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

TNode* insert(TNode* node, int key)
{
	/* 1. Perform the normal BST insertion */
	if (node == NULL)
		return(newNode(key));

	if (key < node->key)
		node->left = insert(node->left, key);
	else if (key > node->key)
		node->right = insert(node->right, key);
	else // Equal keys are not allowed in BST
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left),
						height(node->right));

	/* 3. Get the balance factor of this ancestor
		node to check whether this node became
		unbalanced */
	int balance = getBalance(node);

	// If this node becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && key < node->left->key)
		return rightRotate(node);
	if (balance < -1 && key > node->right->key)
		return leftRotate(node);
	if (balance > 1 && key > node->left->key)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}
	if (balance < -1 && key < node->right->key)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}
	return node;
}

void preOrder(TNode *root)
{
	if(root != NULL)
	{
		printf("%d ", root->key);
		preOrder(root->left);
		preOrder(root->right);
	}
}

TNode *Search(TNode *root, int key)
{
    TNode *p = root;
    while((p!=NULL)&&(key!=p->key))
    {
        if (key <p->key)
        {
            p = p->left;
        }
        else
        {
            p = p->right;
        }

    }
    return p;
}

TNode *FIND(TNode *tree,TNode *n)
{
    TNode *retval = n->root;   
    if (n->key == n->root->key)
    {
        retval = n;
    }
    else
    {
        n->root = FIND(tree,n->root);
    }	    
    return retval;
}

TNode *UNION(TNode *tree, int A , int B)
{
	TNode *retval = NULL; 	// assume both are already same...	
	TNode *nodeA = Search(tree,A);
	TNode *nodeB = Search(tree,B);
	if(nodeA != NULL && nodeB != NULL)
	{
		TNode *nodeA_root = FIND(tree,nodeA);	
		TNode *nodeB_root = FIND(tree,nodeB);		

		if(nodeA_root != nodeB_root)
		{
			if(nodeA_root->S_height > nodeB_root->S_height)
			{
				nodeB_root->root = nodeA_root;
				retval = nodeA_root;
			}
			else
			{
				nodeA_root->root = nodeB_root;
				retval = nodeB_root;
				if(nodeA_root->S_height == nodeB_root->S_height)
				{
					nodeB_root->S_height = nodeB_root->S_height + 1 ;
				}
			}
		}
	}
	return retval;
}

int areInSameSet(TNode *tree, int A , int B)
{
	int result = 0;
	TNode *A_node = Search(tree,A);
	TNode *B_node = Search(tree,B);
	if(A_node != NULL && B_node != NULL)
	{
		if(FIND(tree,A_node) == FIND(tree,B_node))
		{
			result = 1;
		}	
	}
	return result;
}

TNode *joinRight(TNode *leftTree , TNode *keyNode , TNode *rightTree )
{
	TNode *LeftOfLeftT = NULL;
	TNode *RightOfLeftT = NULL;
	if(leftTree != NULL)
	{
		LeftOfLeftT = leftTree->left;
		RightOfLeftT = leftTree->right;

	}
	if( height(RightOfLeftT) - height(rightTree) <=  1 )
	{
		keyNode->left = RightOfLeftT;
		keyNode->right = rightTree;
		keyNode->height = max(height(keyNode->left) , height(keyNode->right));

		if(height(keyNode) - height(LeftOfLeftT)  <=  1)
		{
			leftTree->right = keyNode;
			leftTree->height = max(height(leftTree->right) , height(leftTree->left));
			return leftTree ;
		}
		else
		{
			keyNode = rightRotate(keyNode);
			leftTree->right = keyNode;
			leftTree->height = max(height(leftTree->right) , height(leftTree->left));
			leftRotate(leftTree);
		}
	} 
	else
	{
		TNode *T = joinRight(RightOfLeftT,keyNode,rightTree);
		leftTree->right = T;
		leftTree->height = max( height(leftTree->right) , height(leftTree->left) );
		
		if( height(T) - height(LeftOfLeftT) <=   1  )
		{
			return leftTree;
		}
		else
		{
			return leftRotate(leftTree);
		}
	}
	
}

TNode *joinLeft(TNode *leftTree , TNode *keyNode , TNode *rightTree )
{
	TNode *LeftOfRightT = NULL;
	TNode *RightOfRightT = NULL;
	if(rightTree!=NULL)
	{
		LeftOfRightT = rightTree->left;
		RightOfRightT = rightTree->right;

	}
	if( height(LeftOfRightT) - height(leftTree) <=  1)
	{
		keyNode->left = leftTree;
		keyNode->right = LeftOfRightT;
		keyNode->height = max(height(keyNode->left) , height(keyNode->right ));

		if( height(keyNode) - height(RightOfRightT)  <=  1 )
		{
			rightTree->left = keyNode;
			rightTree->height = max(height( rightTree->right ), height(rightTree->left));
			return rightTree ;
		}
		else
		{
			keyNode = leftRotate(keyNode);
			rightTree->left = keyNode;
			rightTree->height = max(height(rightTree->right) , height(rightTree->left));
			rightRotate(rightTree);
		}
	}  
	else
	{
		TNode *T = joinLeft(leftTree,keyNode,LeftOfRightT);
		rightTree->left = T;
		rightTree->height = max(height(rightTree->right) , height(rightTree->left));
		if(height(T) - height(RightOfRightT) <=   1 )
		{
			return rightTree;
		}
		else
		{
			return rightRotate(rightTree);
		}
	}
}

TNode *join(TNode *leftTree , TNode *keyNode , TNode *rightTree )
{
	TNode *retval = keyNode;
	if( height(leftTree) > height(rightTree) + 1)
	{
		retval = joinRight(leftTree , keyNode , rightTree);
	}
	else if( height(leftTree) + 1  < height(rightTree) )
	{
		retval = joinLeft(leftTree , keyNode , rightTree);	//...
	}
	else // balanced...
	{
		keyNode->left = leftTree;
		keyNode->right = rightTree;
		keyNode->height = max(height(leftTree) , height(rightTree)); 
		retval = keyNode;
	}
 	return retval;
}


int split(TNode *tree , int key , TNode **LSubTree , TNode **RSubTree)
{
	int retval = 0;
	if(tree == NULL)
	{
		*LSubTree = NULL;
		*RSubTree = NULL;
	}
	else if(key < tree->key)
	{
		retval = split(tree->left , key , LSubTree , RSubTree);
		TNode *r = join(*RSubTree , tree , tree->right); //...
		*RSubTree = r;
	}
	else if(tree->key < key)
	{
		retval = split(tree->right , key , LSubTree , RSubTree);
		TNode *k = join(tree->left , tree , *LSubTree); 
		*LSubTree = k;
	}
	else if(tree->key == key)
	{
		*LSubTree = tree->left;
		*RSubTree = tree->right;
		retval = 1;
	}
	return retval;
}

TNode *UNIONofSets(TNode *t1 , TNode *t2)
{
	TNode *retval;
	if(t1==NULL)
	{
		retval = t2;
	}
	else if(t2 == NULL)
	{
		retval = t1;
	}
	else
	{
		TNode *LSubTree,*RSubTree;
		split(t2 , t1->key , &LSubTree , &RSubTree);
		retval = join( UNIONofSets(t1->left,LSubTree) , t1 , UNIONofSets(t1->right,RSubTree) );
	}
	return retval;
}

TNode *join2( TNode *LeftCommon ,  TNode *RightCommon )
{
	TNode *retval ;
	if(LeftCommon == NULL)
	{
		retval = RightCommon;
	}
	if(RightCommon == NULL)
	{
		retval = LeftCommon;
	}
	else
	{
		retval = join( join2(RightCommon->left , LeftCommon ) , RightCommon ,RightCommon->right );
	}
	return retval;
}

TNode *Intersection(TNode *t1 , TNode *t2)
{
	TNode *retval = NULL;
	if(t1 != NULL && t2 != NULL )
	{
		TNode *LSubTree , *RSubTree ;
		int status;
		status = split(t2 , t1->key , &LSubTree , &RSubTree);
		TNode *LeftCommon = Intersection(t1->left , LSubTree);
		TNode *RightCommon = Intersection(t1->right , RSubTree);
		if(status == 1)
		{
			retval = join( LeftCommon , t1 , RightCommon);
		}
		else
		{
			retval = join2( LeftCommon , RightCommon );
		}
	}
	return retval;
}

TNode *differenceAB(TNode *t1 , TNode *t2)
{
	TNode *retval = NULL;
	if(t1 == NULL)
	{
		retval = NULL;
	}
	if(t2 == NULL)
	{
		retval = t1;
	}
	else if(t1 != NULL && t2 != NULL )
	{
		TNode *LSubTree , *RSubTree ;
		int status;
		status = split(t2 , t1->key , &LSubTree , &RSubTree);
		TNode *LeftCommon = differenceAB(t1->left , LSubTree);
		TNode *RightCommon = differenceAB(t1->right , RSubTree);
		if(status == 0)
		{
			retval = join( LeftCommon , t1 , RightCommon);
		}
		else
		{
			retval = join2( LeftCommon , RightCommon );
		}
	}
	return retval;

}

TNode *copyTree(TNode *r , TNode *tree)
{	
	if(tree != NULL)
	{
		r = insert(r , tree->key);
		r = copyTree(r , tree->left);
		r = copyTree(r , tree->right);
	}
	return r;
}

TNode *SymmetricDifference(TNode *t1 , TNode *t2)
{
	TNode *x1 = NULL , *x2 = NULL;
	x1 = copyTree(x1,t1);
	x2 = copyTree(x2,t2);

	t1 = differenceAB(t1,t2);
	t2 = differenceAB(x2,x1);
	return UNIONofSets(t1,t2);
}

int main()
{
TNode *root = NULL;

int select , stop = 0 , stop1 = 0, select1 , x, y , status , n , m , i ,sizeA , sizeB , temp;
TNode *result;


	while(stop == 0)
	{
		printf("\n1. FIND , UNION , Integers belongs to same set \n2. Set Operations \n3. Exit\n");
		scanf("%d",&select);
		switch(select)
		{
			case 1 :
					{
						printf("Enter size of set \n");
						scanf("%d",&n);

						printf("\nEnter Elements\n");
						for (i = 0; i < n; ++i)
						{
							scanf("%d",&temp);
							root = insert(root,temp);
						}

						stop1 = 0;
						while(stop1 == 0)
						{
							printf("\n1. FIND\n2. UNION\n3. Integers belong to same set \n4. Stop\n");
							scanf("%d",&select1);
							switch(select1)
							{
								case 1:
										printf("Enter Element from given set to find\n");
										scanf("%d",&x);
										TNode *s = Search(root , x);
										if(s != NULL)
										{
											result = FIND(root, s);	
											if(result)
											{
												printf("Root %d\n", result->root->key);
											}
										}
										else
                                        {
                                            printf("WRONG INPUT\n");
                                        }
										break;

								case 2:
										printf("\nUNION(A,B)\nA = ");
										scanf("%d",&x);
										printf("B = ");
										scanf("%d",&y);
										result = UNION(root,x,y);
										if(result != NULL)
										{
											printf("New root %d\n", result->root->key);
										}
										else
										{
											printf("Both Elements are already in same subset\n");
										}
										break;

								case 3 :
										printf("\nEnter two Integers from given set\n");
										scanf("%d%d",&x,&y);
										status = areInSameSet(root , x , y );
										if(status == 1)
										{
											printf("Both Integers are in same set\n");
										}
										else
										{
											printf("Both Integers are in different set\n");
										}
										break;

								case 4:
										stop1 = 1;
										break;

								default:
										printf("Please select valid options\n");
										break;
							}
						}
						break;
					}

			case 2 :
					{
						TNode *t1  , *t2  ;
						stop1 = 0;
						while(stop1 == 0)
						{
							printf("\n\nPerform following operations on subsets A and B\n");
							printf("1. Intersection of A and B\n2. (A U B) \n3. (A - B) \n4. (A - B) U (B - A)\n5. stop\n");
							scanf("%d",&select1);
							if(select1 != 5)
							{
								t1 = NULL;
								t2 = NULL;					
								printf("\nCreate listA\n");
								printf("size of listA\n");
								scanf("%d",&sizeA);
								printf("Enter listA\n");
								for(i=0;i<sizeA;i++)
								{
									int val;
									scanf("%d",&val);
									t1 = insert(t1,val);
								}

								printf("\nCreate listB\n");
								printf("size of second list\n");
								scanf("%d",&sizeB);						
								printf("Enter listB\n");
								for(i=0;i<sizeB;i++)
								{
									int val;
									scanf("%d",&val);
									t2 = insert(t2,val);
								}	
							}							
							
							switch(select1)
							{
								case 1:
										{
											result = Intersection(t1 , t2);
											printf("Intersection of A and B\n");
											preOrder(result);
											break;
										}
								case 2:
										{

											result =  UNIONofSets( t1 , t2 );
 											printf("\n(A U B)\n");
 											preOrder(result);
											break;
										}

								case 3:
										{
											result = differenceAB(t1,t2);
 											printf("\n(A - B)\n");
 											preOrder(result);
											break;
										}

								case 4:
										{
											result = SymmetricDifference(t1 , t2) ;
 											printf("\n(A - B) U (B - A)\n");
 											preOrder(result);
											break;
										}


								case 5:
										stop1 = 1;
										break;
										
								default:
										printf("Please select valid options\n");
										break;
							}
						}

						break;
					}

			case 3 :
					stop = 1;
					break;

			default :
						printf("Please select valid options\n");
						break;

		}
	}


return 0;
}
