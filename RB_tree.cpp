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

    RBTreeNode temp_color;
    temp_color.color=tmp->color;

    tmp->color=node->color;
    node->color=temp_color.color;
    /*only deletion case*/
    // if(node->left_child)
    //     node->leftchild->color=RED;
    return tmp;
}



RBTreeNode* RB_tree::left_local_rotation(RBTreeNode* node)
{
    

    RBTreeNode* tmp=node->right_child;
    node->right_child=tmp->left_child;
    tmp->left_child=node;

    RBTreeNode temp_color;
    temp_color.color=tmp->color;
    
    tmp->color=node->color;
    node->color=temp_color.color;
    /*only deletion case*/
    // if(node->right_child)
    //     node->right_child->color=RED;
    return tmp;
}


RBTreeNode* RB_tree::left_rotation_without_change_color(RBTreeNode* node)
{
    RBTreeNode* tmp=node->right_child;
    node->right_child=tmp->left_child;
    tmp->left_child=node;
    return tmp;
}

RBTreeNode* RB_tree::right_rotation_without_change_color(RBTreeNode* node)
{
    RBTreeNode* tmp=node->left_child;
    node->left_child=tmp->right_child;
    tmp->right_child=node;
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
    if(NULL==node) {cout<<"flip error!!"<<endl;return NULL;};
    node->color = !node->color;
    if(node->left_child)
        node->left_child->color = !node->left_child->color;
    if(node->right_child)
        node->right_child->color =!node->right_child->color;
    return node;
}



/**
    cases for unbalanced:
**/
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


/****************************************deletion*****************************************/
/*****************************************************************************************/

void RB_tree::swap_two_node(RBTreeNode* dst,RBTreeNode* src)
{
    if((NULL==dst) || (NULL==src)) throw("swap node--NULL param!!");
    RBTreeNode temp;
    temp.key=dst->key;
    temp.value=dst->value;

    dst->key=src->key;
    dst->value=src->value;

    src->key=temp.key;
    src->value=temp.value;
}

RBTreeNode* RB_tree::delete_less_than_two_child(RBTreeNode* node,del_stat_t &stat)
{
    /*in delete cases,if node's color is red,it will not have any child*/
    if(RED == node->color)
    {
        delete node;
        return NULL;
    }
    else if(BLACK == node->color)
    {
        if(is_red(node->right_child))
        {
            swap_two_node(node,node->right_child);
            delete node->right_child;
            node->right_child=NULL;
            return node;
        }
        /*this case will not appear in delete min*/
        else if(is_red(node->left_child))
        {
            swap_two_node(node,node->left_child);
            delete node->left_child;
            node->left_child=NULL;
            return node;
        }
        else
        {
            //cout<<"black"<<endl;
            stat=DELETE_LEFT_BLACK;
            delete node;
            return NULL;
        }
    }
    else
    {
        throw("It can't be hear-2");
    }
    throw("It can't be hear-3");
    return NULL;
}


/*插入的情况：父节点为黑，直接插入
            父节点为红，这时候祖父肯定为黑：
                                        如果叔叔为红，将父节点和叔叔节点变成黑，祖父节点变成红色。
                                        如果叔叔为黑，则需要进行旋转（可能需要先局部旋转）
//已完成
*/


/*删除最小值的情况：被删除节点为红节点，直接删除(因为不可能会存在子节点)
                 被删除节点为黑节点：
                                如果有红色子节点，用子节点代替，再删除子节点(存在右子节点，不存在左子节点，因为左子节点是最小)
                                被删除节点没有子节点：
                                    一、被删除节点的父节点为红色的处理，兄弟可能有红子节点或者没有，分情况讨论：   //这一情况未完成
                                        （1）被删除节点的兄弟节点为黑色，存在左红子或右红子，但是不同时存在
                                        （2）被删除节点的兄弟节点为黑色，同时存在左右红色子节点
                                        （3）被删除节点的兄弟节点为黑色，没有子节点
                                    二、被删除节点的父节点为黑色节点，分情况讨论：
                                        （1）父亲为黑，兄弟为红，进行旋转，但是这个时候的旋转并不能达到平衡，旋转后的结果为情况一，需要再进行处理   //这一情况未完成
                                        （2）父节点为黑，兄弟节点为黑，兄弟节点有红色子节点，需要旋转达到平衡
                                        （3）父节点为黑，兄弟节点为黑，且兄弟节点没有红色节点，将兄弟节点染红，即这个分支的黑节点数量减一，向上传递，重新地递归考虑上述情况。  //这一情况未完成
            ！！！！需要重写旋转平衡函数
*/


RBTreeNode* RB_tree::delete_black_node(RBTreeNode* root,del_stat_t &stat)
{
    if(NULL!=root->left_child)
    {
        cout<<"root="<<root->key<<endl;
        cout<<"delete black node error!!"<<endl;
    }
    /*case1:The deleted node's  father is red.*/
    if(is_red(root))
    {
        root=flip(root);
        stat= NOTHING;
        return root;
    }
    
    else if(!is_red(root))
    {
        /*case2:The deleted node's  father is black,the brother is red,rotation. */
        if(is_red(root->right_child))
        {
            // if(root->right_child->left_child)
            //     root->right_child->left_child->color=RED;
            root=left_local_rotation(root);
            root->left_child=left_rotation_without_change_color(root->left_child);
            // root->left_child=flip(root->left_child);
            // root->left_child=left_local_rotation(root->left_child);
            // root->left_child->color=BLACK;
            stat=NOTHING;
        }
        /*case3:The deleted node's  father is black,the brother is black*/
        else if(!is_red(root->right_child))
        {
            /*case 3-1:the brother node has red right_child.rotation.*/
            if( is_red(root->right_child->right_child))
            {
                root->right_child->right_child->color=BLACK;
                root=left_local_rotation(root);
                stat=NOTHING;
            }
            /*case3-2:The deleted node's  father is black,the brother is black,brother's left_child is red,no right_child*/
            else if( (is_red(root->right_child->left_child)) && ((!is_red(root->right_child->right_child))) )
            {
                root->right_child=right_local_rotation(root->right_child);
                root=left_local_rotation(root);
                root->right_child->color=BLACK;
                stat=NOTHING;
            }
            /*case3-3:The deleted node's  father is black,has no child.*/
            else if((NULL==root->right_child->left_child) && (NULL==root->right_child->right_child))
            {
                root->right_child->color=RED;
                stat=PASS_UP;
            }
        }
    }
    return root;
}

RBTreeNode* RB_tree::fix_left_side_shoter_case(RBTreeNode* root,del_stat_t &stat)
{
    /*case1:The deleted node's  father is red,just flip can keep balance.*/
    if(is_red(root))
    {
        root->left_child->color=!root->left_child->color;
        root=flip(root);
        stat=NOTHING;
    }
    else if(!is_red(root))
    {
        if(is_red(root->right_child))
        {
            root=left_local_rotation(root);
            root->left_child=left_rotation_without_change_color(root->left_child);

            stat=NOTHING;
        }
        else if(!is_red(root->right_child))
        {
            if( is_red(root->right_child->right_child))
            {
                root->right_child->right_child->color=BLACK;
                root=left_local_rotation(root);
                stat=NOTHING;
            }
            else if( (is_red(root->right_child->left_child)) && ((!is_red(root->right_child->right_child))) )
            {
                root->right_child=right_local_rotation(root->right_child);
                root=left_local_rotation(root);
                stat=NOTHING;
            }
            else if((NULL==root->right_child->left_child) && (NULL==root->right_child->right_child))
            {
                root->right_child->color=RED;
                stat=PASS_UP;
            }
        }
    }
}



RBTreeNode* RB_tree::check_stat(RBTreeNode* root,del_stat_t& stat)
{
    switch(stat)
    {
        case NOTHING:break;
        case DELETE_LEFT_BLACK:
            
            root=delete_black_node(root,stat);
        break;
        case PASS_UP:
            cout<<"fuck hear!!"<<endl;
            cout<<"root="<<root->key<<endl;
            root=fix_left_side_shoter_case(root,stat);
            
        break;
        default:break;
    }
    return root;
}

RBTreeNode* RB_tree::delete_min()
{   
    del_stat_t stat=NOTHING;
    if(is_empty()) throw("empty tree!!");
    root=delete_min(root,stat);
    root->color=BLACK;
    return root;
}





RBTreeNode* RB_tree::delete_min(RBTreeNode* root,del_stat_t &stat)
{
    if(NULL==root->left_child)
    {
        //cout<<root->key<<endl;
        /*two cases:node's color is red,right_child's is red*/
        /*left_child && right_child are red is impossible.*/
        root=delete_less_than_two_child(root,stat);
        return root;
    }
    
    root->left_child=delete_min(root->left_child,stat);
    root=check_stat(root,stat);
    return root;
}



/**********************************debug interface***********************************/
/**********************************debug interface***********************************/
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
    u32 data[]={9,18,17,6,5,14,3,12,1,4,4,32,45,67,87,65,45,67,6,43,45,67,23,17,84,36,367,4367,7,5,54,54,67,455};
    //u32 data[]={2,4,6,8,10,12,14,16,18};
    for(int i=0;i<sizeof(data)/4;i++)
    {
        root=rb.insert_node(data[i],5);
    }

    rb.level_traversal(root);

    // for(int i=0;i<sizeof(data)/4-1;i++)
    // {
    //     root=rb.delete_min();
    // }

     root=rb.delete_min();
     root=rb.delete_min();
    // root=rb.delete_min();
    // root=rb.delete_min();
    // root=rb.delete_min();
    // root=rb.delete_min();
    
    //root=rb.insert_node(3,5);
    
    rb.level_traversal(root);

    rb.check_rb_tree_balance(root);
}




