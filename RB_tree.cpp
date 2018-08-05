#include "RB_tree.h"





bool RB_tree::is_red(RBTreeNode* node)
{
    if(NULL==node) return false;
    return node->color==RED;
}



RBTreeNode* RB_tree::right_local_rotation(RBTreeNode* node)
{
    RBTreeNode* tmp=node->left_child;
    node->left_child=tmp->right_child;
    tmp->right_child=node;
    

    tmp->color=node->color;
    node->color=RED;
    return tmp;
}



RBTreeNode* RB_tree::left_local_rotation(RBTreeNode* node)
{
    RBTreeNode* tmp=node->right_child;
    node->right_child=tmp->left_child;
    tmp->left_child=node;
    
    tmp->color=node->color;
    node->color=RED;
    return tmp;
}






bool RB_tree::continu_red(RBTreeNode* node)
{
    if(is_red(node) && (is_red(node->left_child) || is_red(node->right_child)))
        return true;
    else 
        return false;
}


RBTreeNode* RB_tree::flip(RBTreeNode* node)
{
    node->color = !node->color;
    node->left_child->color = !node->left_child->color;
    node->right_child->color =!node->right_child->color;
    return node;
}


RBTreeNode* RB_tree::check_balance(RBTreeNode* node)
{
    if(continu_red(node->left_child))
    {
        if(is_red(node->right_child))
            node=flip(node);
        else
        {
            if(is_red(node->left_child->left_child))
            {
                node=right_local_rotation(node);
            }
            else if(is_red(node->left_child->right_child))
            {
                node->left_child=left_local_rotation(node->left_child);
                node=right_local_rotation(node);
            }
        }
    }
    else if(continu_red(node->right_child))
    {
        if(is_red(node->left_child))
            node=flip(node);
        else
        {
            if(is_red(node->right_child->left_child))
            {
                node->right_child=right_local_rotation(node->right_child);
                node=left_local_rotation(node);
            }
            else if(is_red(node->right_child->right_child))
            {
                node=left_local_rotation(node);
            }
        }
    }
    return node;
}


RBTreeNode* RB_tree::new_node(KEY key,VAL value)
{
    RBTreeNode* tmp=NULL;
    try
    {
        tmp=new RBTreeNode;
    }
    catch(const std::bad_alloc &e)
    {
        print_error("malloc failed!!!");
    }
    tmp->key=key;
    tmp->value=value;
    return tmp;
}

RBTreeNode* RB_tree::insert_node(RBTreeNode* root,KEY key,VAL value)
{
    if(NULL==root)
    {
        return new_node(key,value);
    }
    if(key > root->key)
    {
        root->right_child=insert_node(root->right_child,key,value);
        root=check_balance(root);
    }
    else if(key < root->key)
    {
        root->left_child=insert_node(root->left_child,key,value);
        root=check_balance(root);
    }
    else
    {
        print_error("DOSE NOT SUPPORT SAME VALUE!!");
    }
    return root;
}


RBTreeNode* RB_tree::insert_node(KEY key,VAL value)
{   
    root=insert_node(root,key,value);
    root->color=BLACK;
    return root;
}


/*Swap two nodes' value*/

/**********************************debug interface***********************************/
QUEUE<RBTreeNode*> Q_leveas;
err_t RB_tree::find_all_leaves(RBTreeNode* root)
{
    if(NULL==root)
    {
        return NO_ERROR;
    }
    find_all_leaves(root->left_child);
    if((NULL==root->left_child)&&(NULL==root->right_child))
    {
        Q_leveas.push(root);
    }
    find_all_leaves(root->right_child);
    return NO_ERROR;
}

err_t RB_tree::find_black_nodes_of_path(RBTreeNode* root,RBTreeNode* node,u32& count)
{
    if(node->key<root->key)
    {
        if(NULL==root->left_child)
        {
            return ERROR_PARAM;
        }
        find_black_nodes_of_path(root->left_child,node,count);
        if(BLACK==root->color)
        {
            count++;
        }
    }
    else if(node->key>root->key)
    {
        if(NULL==root->right_child)
        {
            return ERROR_PARAM;
        }
        find_black_nodes_of_path(root->right_child,node,count);
        if(BLACK==root->color)
        {
            count++;
        }
    }
    else
    {
        if(BLACK==root->color)
        {
            count++;
        } 
    }
    return NO_ERROR;
}


//check if every path of leaf to root has the same number of black nodes. 
err_t RB_tree::check_rb_tree_balance(RBTreeNode* root)
{

    RBTreeNode* temp;
    u32 counts=0;
    find_all_leaves(root);
    
    while(!Q_leveas.pop(temp))
    {
        find_black_nodes_of_path(root,temp,counts);
        cout<<counts<<endl;
        counts=0;
    }
    return NO_ERROR;
}


/************************************************************************************/

RB_tree rb;
int main()
{
    RBTreeNode* root=NULL;
    u32 data[]={9,18,17,6,5,14,3,12,1,4};
    for(int i=0;i<10;i++)
    {
        root=rb.insert_node(data[i],5);
    }
    rb.level_traversal(root);
}




