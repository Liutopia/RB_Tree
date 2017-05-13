
// change this to your id
static const char* student_id = "0416235" ;
#include <vector>
#include <deque>
#include <array>
#include <iostream>
#include <iomanip>
using namespace std;
#define BLACK 0
#define RED 1

// do not edit prototype
void Insert(int *, int);
void Delete(int *, int);
int Select(int *, int);
int Rank(int *, int);

// data structure :
// tree is an array with enough space
// tree[0] is the array size
// bundle three attribute as a node data
// First attribute is color, "0" means black, "1" means red , NIL is also "0", "-1" means no data
// Second attribute is key, "0" means NIL, "1"~"999" means data,  "-1" means no data
// Third attribute is size, for Dynamic Order Statistics, "0" means NIL , "-1" means no data
//
// for example,
// if tree[0] is "256" says that the array size is 256
//
// if tree[1] is "1" says that the place of 1 is a red node
//
// if tree[2] is "5" says that the place of 1 is key of 5
//
// if tree[8] is "-1" says that the place of 3 has nothing
//
// if tree[14] is "0" says that the place of 5 is a node of NIL
//
// if tree[19] is "66" says that the place of 7 is key of 66

//
// if there is an ambiguous situation, choose the smaller ( left ) one
//

//
// do your homework here
//
class node{
	public:
    node() { color=-1;    value=-1;     size=-1;    total_size=0;
             parent=0;    left=0;       right=0;    root=0;
	         set_nil();   set_empty();  set_marked(); }

    node(int n1, int n2, int n3) {   color=n1;    value=n2;     size=n3;     total_size=0;
                                     parent=0;    left=0;       right=0;     root=0;
                                     set_nil();   set_empty();  set_marked(); }

	void set_nil()    { nil    = (color== 0) & (value== 0) & (size== 0); }
	void set_empty()  { empty  = (color==-1) & (value==-1) & (size==-1); }
	void set_marked() { marked = ( nil == 0) & (empty== 0); }

	//~node() {delete parent,left,right;}

	int color;
	int value;
	int size;
	int total_size;//record in RBtree[0]

	node* parent;
	node* left;
	node* right;
	node* root;//delete 要重指root


	bool nil;// = (color== 0) & (value== 0) & (size== 0);
	bool empty;// = (color==-1) & (value==-1) & (size==-1);
	bool marked;// = ( nil == 0) & (empty== 0);
};

void Array_to_BST(int* array, std::deque<node>& RBtree){


	//first node of deque is nil node
	RBtree.push_back(node(0,0,0));
	RBtree[0].total_size=array[0];
	//if now RBtree is empty, then push nil into root
	if(!array)       {RBtree.push_back(node(0,0,0)); RBtree[0].root=&RBtree[1]; return;}
	if(array[1]==-1) {RBtree.push_back(node(0,0,0)); RBtree[0].root=&RBtree[1]; return;}


	//push array to deque
	for(int i=1;i<array[0] ;i=i+3){

		RBtree.push_back( node(array[i],array[i+1],array[i+2]) );
	}

	//build the links
	int L=0,R=0;

	for(int i=1;i<RBtree.size();++i){
		L=i*2;
		R=L+1;
		if(RBtree[i].marked && L<RBtree.size() && R<RBtree.size()){
			RBtree[i].parent = &RBtree[i/2];
			RBtree[i].left   = &RBtree[L];
			RBtree[i].right  = &RBtree[R];
		}
		else if(RBtree[i].nil){
			RBtree[i].parent = &RBtree[i/2];

		}

	}

	RBtree[0].root=&RBtree[1];
	return;
}

int height(node* node){
    if (node==0)
        return 0;

    else {
        // compute the height of each subtree
        int lheight = height(node->left);
        int rheight = height(node->right);

        // use the larger one
        if (lheight > rheight)
            return(lheight+1);
        else
			return(rheight+1);
    }
}

void Level_Order_Traversal(node* root, int level, int* array, int* idx){

	if(root==0)
		return;

    if (level==1) {

		array[*idx]   = root->color;
		array[*idx+1] = root->value;
		array[*idx+2] = root->size;
		*idx = (*idx)+3;
	}


    else if (level > 1) {

		if(root->nil) {
			*idx = (*idx)+6*(level-1);
			return;
		}
        Level_Order_Traversal(root->left,  level-1, array, idx);
        Level_Order_Traversal(root->right, level-1, array, idx);
    }
	return;
}

void BST_to_Array(std::deque<node>& RBtree, int* array){

	//array[0] is the size
	array[0]=RBtree[0].total_size;

	for(int i=1;i<array[0];++i)
		array[i]=-1;


	//use level-order traversal to write back BST
	if(RBtree.size()==2) return;

	int h = height(RBtree[0].root);
	//cout<<"h="<<h<<endl;

	int* idx=new int(1);

    for (int level=1; level<=h; ++level)
        Level_Order_Traversal(RBtree[0].root, level, array, idx);


	delete idx;
	return;

}

void Left_Rotate(std::deque<node>& RBtree, node& x){


	node* temp = x.right;			//set temp link to x.right
	x.right    = temp->left;		//turn temp's left subtree to x.right

	if(!(temp->left->nil))
		temp->left->parent=&x;

	temp->parent=x.parent;			//link x'parent to temp

	//if(x.parent->nil)				//x is root
	if(&x == RBtree[0].root)
        RBtree[0].root = temp;


	else if(&x==x.parent->left)		//x is some node's left child
		x.parent->left  = temp;
	else 							//x is some node's right child
		x.parent->right = temp;

	temp->left = &x;					//put x on temp's left
	x.parent = temp;
		//cout<<"temp.value="<<temp->value<<endl;
		//cout<<"root.value = "<<root->value<<endl;
	return;
}

void Right_Rotate(std::deque<node>& RBtree, node& x){

	node* temp = x.left;			//set temp link to x.left
	x.left     = temp->right;		//turn temp's right subtree to x.left

	if(!(temp->right->nil))
		temp->right->parent=&x;

	temp->parent=x.parent;			//link x'parent to temp

	if(x.parent->nil)				//x is root
		RBtree[0].root = temp;
	else if(&x==x.parent->left)	//x is some node's left child
		x.parent->left  = temp;
	else 							//x is some node's right child
		x.parent->right = temp;

	temp->right = &x;				//put x on temp's left
	x.parent = temp;

	return;
}

void RB_Insert_Fixup(std::deque<node>& RBtree, node * z){

	node * y=0;// y is z's uncle
	while(z->parent->color==RED){
		//case 1
		//z is in left subtree of z's grandparent
		if(z->parent == z->parent->parent->left){

			y=z->parent->parent->right;
			if(y->color==RED){
				//case 1-1
				//z’s uncle y is red.
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z=z->parent->parent;
			}
			else {

				if(z==z->parent->right) {
					//case 1-2
					//z’s uncle y is black and z is a right child.
					z=z->parent;
					Left_Rotate(RBtree,*z);
				}
				//case 1-3
				//z’s uncle y is black and z is a left child.
				z->parent->color=BLACK;
				z->parent->parent->color=RED;
				Right_Rotate(RBtree,*(z->parent->parent));
			}

		}
		//case 2
		//z is in right subtree of z's grandparent

		else {

			y=z->parent->parent->left;
			if(y->color==RED){
				//case 2-1
				//z’s uncle y is red.
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color=RED;
				z=z->parent->parent;
			}
			else {

				if(z==z->parent->left){
					//case 2-2
					z=z->parent;
					Right_Rotate(RBtree,*z);
				}

				//case 2-3
				z->parent->color         = BLACK;
				z->parent->parent->color = RED;

				Left_Rotate(RBtree,*(z->parent->parent));

			}
		}

	} //while loop end
	RBtree[0].root->color=BLACK;


	return;
}

int Size_Value(node * node){

	//DP?

	//node points to empty or nil
    if (node==0)
        return 0;

	else if(node->nil)
		return 0;

	//node points to marked leaf node
	else if(node->left==0 && node->right==0)
		return 1;

	//node points to marked nonleaf node
    else
        return 1 + (Size_Value(node->left)) + (Size_Value(node->right));
	

}

void Size_Fixup(node * node){
	
	if(node==0) return;
	if(node->nil) return;
	
	node->size = Size_Value(node);
	Size_Fixup(node->left);
	Size_Fixup(node->right);
	
	return;
} 

void Insert(int * tree, int key) {

	//for(int i=0;i<190;++i) cout<<"insert_tree["<<i<<"]="<<tree[i]<<endl;
	//tree
	if(tree[1]==-1){
		tree[1]=0;
		tree[2]=key;
		tree[3]=1;

		tree[4]=0;
		tree[5]=0;
		tree[6]=0;

		tree[7]=0;
		tree[8]=0;
		tree[9]=0;
		return;
	}

//for(int i=0;i<10;++i) cout<<"in Insert func\n"<<"insert_tree["<<i<<"]="<<tree[i]<<endl;
	std::deque<node> RBtree;
	Array_to_BST(tree,RBtree);

	node* z = new node(1,key,1);
	node* y = &RBtree[0];	 //y=nil
	node* x = RBtree[0].root;//x=root

	// if(x!=nil)
	while(x->nil==0){

		y=x;
		if(z->value < x->value)
			x = x->left;
		else
			x = x->right;
	}

	z->parent=y;
	if(y->nil)
		//RBtree[1]=*z;
		RBtree[0].root=z;
	else if((z->value) < (y->value))
		y->left=z;
	else
		y->right=z;


	z->left = &RBtree[0];
	z->right = &RBtree[0];
	z->color = 1;	//z.color = red*/


    RB_Insert_Fixup(RBtree,z);

	//size_fixup
	Size_Fixup(RBtree[0].root);

	

	BST_to_Array(RBtree,tree);
	delete z;
	return;
}

node * Binary_Search(std::deque<node>& RBtree, int key){

	node * temp = RBtree[0].root;
	while(temp!=0){

		if(key == temp->value) {
			return temp;
		}
		else if (key < temp->value)
			temp = temp->left;
		else
			temp = temp->right;
	}

	return 0;
}

void RB_Transplant(std::deque<node>& RBtree, node * u, node * v){
	

	if(u==0)    return;
	if(v==0)    return;

	if(u->parent->nil)
		RBtree[0].root = v;
	else if(u==u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;

	v->parent = u->parent;

}

void RB_Delete_Fixup(std::deque<node>& RBtree, node* x){
	
	node* w = 0;
	
	while(x!=RBtree[0].root && x->color==BLACK){
		if(x==x->parent->left){
			// w is x's sybling
			w = x->parent->right;
			if(w->color==RED){
				//case 1
				//w is red
				w->color = BLACK;
				x->parent->color = RED;
				Left_Rotate(RBtree, *(x->parent));
				w = x->parent->right;
			}
			if(w->left->color==BLACK && w->right->color==BLACK){
				//case 2
				//w's two children are both black
				w->color = RED;
				x = x->parent;
			}
			else {
				//case 3
				if(w->right->color == BLACK){
					w->left->color = BLACK;
					w->color = RED;
					Right_Rotate(RBtree, *w);
					w = x->parent->right;
				}
				//case 4
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				Left_Rotate(RBtree,*(x->parent));
				x = RBtree[0].root;
			}
		}
		else{
			//x is the right child of its parent
			
			// w is x's sybling
			w = x->parent->left;
			if(w->color==RED){
				//case 1
				w->color = BLACK;
				x->parent->color = RED;
				Right_Rotate(RBtree, *(x->parent));
				w = x->parent->left;
			}
			if(w->left->color==BLACK && w->right->color==BLACK){
				//case 2
				//w's two children are both black
				w->color = RED;
				x = x->parent;
			}
			else {
				//case 3
				if(w->left->color == BLACK){
					w->right->color = BLACK;
					w->color = RED;
					Left_Rotate(RBtree, *w);
					w = x->parent->left;
				}
				//case 4
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				Right_Rotate(RBtree,*(x->parent));
				x = RBtree[0].root;
			}
		}
		
	}//while loop end
	
	x->color = BLACK;
	
	return;
}

node * Tree_Minimum(node * root){

	while(!root->left->nil)
		root = root->left;

	return root;
}

node * Tree_Maximum(node * root){

	while(!root->right->nil)
		root = root->right;

	return root;
}

void Delete(int * tree, int key) {

	std::deque<node> RBtree;
	Array_to_BST(tree,RBtree);

	
	node* z = Binary_Search(RBtree, key);
	
	//if fail to search, no need to delete
	if(z==0) 
		return;
	
	node* y = z;
	node* x = 0;
	int y_orig_color = y->color;
	
	//if z has only right child
	if(z->left->nil){
		x = z->right;
		RB_Transplant(RBtree,z,z->right);
	}
	else if(z->right->nil){
		x = z->left;
		RB_Transplant(RBtree,z,z->left);
	}
	else {
		//y = Tree_Minimum(z->right);
		y = Tree_Maximum(z->left);
		y_orig_color = y->color;
		//x = y->right;
		x = y->left;
		
		
		if(y->parent==z){
			x->parent = y;
		}
			

		else {
			RB_Transplant(RBtree,y,y->left);
			y->left = z->left;
			y->left->parent=y;
			//RB_Transplant(RBtree[0].root,y,y->right);
			//y->right = z->right;
			//y->right->parent = y;
		}
		RB_Transplant(RBtree,z,y);
		
		y->right = z->right;
		y->right->parent = y;
		//y->left         = z->left;
		//y->left->parent = y;
		y->color        = z->color;
	}
	if(y_orig_color==BLACK)
		RB_Delete_Fixup(RBtree,x);
	//size_fixup
	Size_Fixup(RBtree[0].root);
	BST_to_Array(RBtree,tree);
	return;
}

int RB_Select(node * x, int i){
	
	int r = Size_Value(x->left) + 1;
	if(i==r)
		return x->value;
	else if (i<r)
		return RB_Select(x->left,i);
	else 
		return RB_Select(x->right,i-r);
}

int Select(int * tree, int i) {
	// use Dynamic Order Statistics to return the i'th smallest element
	
	std::deque<node> RBtree;
	Array_to_BST(tree,RBtree);
	
	node * x = RBtree[0].root;
	
	return RB_Select(x, i);
}



int Rank(int * tree, int x) {
	// use Dynamic Order Statistics to return the rank of element x in the tree
	
	std::deque<node> RBtree;
	Array_to_BST(tree,RBtree);
	
	node * temp = Binary_Search(RBtree, x);
	
	if(temp==0) return 0;
	
	int r = Size_Value(temp->left)+1;
	node * y = temp;
	
	while(y!=RBtree[0].root){
		if(y==y->parent->right)
			r = r + y->parent->left->size + 1;
		y = y->parent;
	}
	return r;
}
