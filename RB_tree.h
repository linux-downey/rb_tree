#ifndef _AVL_TREE_H
#define _AVL_TREE_H



#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

#include "math.h"
#include "binary.h"

#ifndef PRE_DEFINE
#define PRE_DEFINE

using std::cout;
using std::cin;
using std::endl;
using std::string;

using u8 =unsigned char;
using u32=unsigned int;
using s32=int;
using u16=unsigned short;

#endif


#define UNBANLANCED_ROOT  1
#define UNBANLANCED_NON_ROOT  2
#define MAX(a,b)  (a>b?a:b)


#define UNBALANCED  1
#define BALANCED    0

#define BLACK  false
#define RED    true

using color_t = bool;



#define CHECK_RESULT(a,b)       do{                                             \
                                    if(a=b)                                     \
                                    {                                           \
                                        cout<<__FILE__<<__LINE__<<endl;         \
                                        cout<<"error code is"<<a;               \
                                        return a;                               \
                                    }                                           \
                                  }while(0)


typedef u32 KEY;
typedef u32 VAL;


class RBTreeNode
{
    public:
        RBTreeNode(){color=RED;left_child=NULL;right_child=NULL;}
        RBTreeNode *left_child;
        RBTreeNode *right_child;
        bool color;
        KEY key;
        VAL value;
};



typedef enum
{
    NO_ERROR,
    ERROR_PARAM=-1,
    NULL_POINTER=-2,
    BAD_ALLOC=-3,
    SAME_KEY=-4,
}err_t;

typedef enum
{
    NOTHING,
    DELETE_LEFT_BLACK,
    PASS_UP,
}del_stat_t;



class RB_tree:public BinaryTree<RBTreeNode>
{
    public:
        RB_tree(){root=NULL;}
        ~RB_tree(){}
        RBTreeNode* get_root(){return root;}
        
        RBTreeNode* insert_node(RBTreeNode* root,KEY key,VAL value);
        RBTreeNode* insert_node(KEY key,VAL value);

        RBTreeNode* right_local_rotation(RBTreeNode* node);
        RBTreeNode* left_local_rotation(RBTreeNode* node);
        RBTreeNode* check_balance(RBTreeNode* node);
        bool continu_red(RBTreeNode* node);

        err_t check_rb_tree_balance(RBTreeNode* root);

        RBTreeNode* delete_black_node(RBTreeNode* root,del_stat_t &stat);
        
        RBTreeNode* delete_min();
        RBTreeNode* fix_left_side_shoter_case(RBTreeNode* root,del_stat_t &stat);

        RBTreeNode* left_rotation_without_change_color(RBTreeNode* node);
        RBTreeNode* right_rotation_without_change_color(RBTreeNode* node);


    private:
        void swap_two_node(RBTreeNode* dst,RBTreeNode* src);
        RBTreeNode* delete_less_than_two_child(RBTreeNode* node,del_stat_t &stat);

        RBTreeNode* delete_min(RBTreeNode* root,del_stat_t &stat);
        RBTreeNode* check_stat(RBTreeNode* root,del_stat_t &stat);

        RBTreeNode* flip(RBTreeNode* node);
        bool is_red(RBTreeNode* node);
        err_t find_all_leaves(RBTreeNode* root);
        err_t find_black_nodes_of_path(RBTreeNode* root,RBTreeNode* node,u32& count);
        bool is_empty(){return !root;}
        RBTreeNode* new_node(KEY key,VAL value);

        void print_error(const char *str){cout<<"error:line "<<__LINE__<<":"<<str<<endl;};
        void print_error(string str){cout<<"error:line "<<__LINE__<<":"<<str<<endl;};
        RBTreeNode *root;
};

#endif
