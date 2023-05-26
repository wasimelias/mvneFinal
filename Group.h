//
// Created by wasim on 21/05/2023.
//


#ifndef WET1_GROUP_H
#define WET1_GROUP_H
#include "BinaryTree.h"
#include "User.h"
#include "wet1util.h"

class Group{

    BinaryTree<int,User> members;
    int groupId;
    int vip_count;
    int views[5];
    int totalViews[5];

public:

    explicit Group(int id);

    virtual ~Group();

    BinaryTree<int,User>& get_members();

    int get_group_id() const ;

    bool get_isVip() const;

    int get_comedy_watches() const;

    int get_drama_watches() const;

    int get_fantasy_watches() const;

    int get_action_watches() const;

    int get_total_watches() const;

    Genre getLovedGenre();

    int* get_views_array();

    int* get_total_views_array();

    void DeleteUserFromGroup(int userId);

    void UpdateStatsAddUser(Node<int,User>* userToAdd);





};

class removeUserFromGroup{
    Group& usersGroup;
public:
    explicit removeUserFromGroup(Group& grp) : usersGroup(grp){}
    void operator()(Node<int,User>* user_to_remove){
        std::shared_ptr<User> RemoveUser = user_to_remove->get_data();
        int* group_views_array = usersGroup.get_views_array();
        RemoveUser->nullify(group_views_array);
    }
};

#endif //WET1_GROUP_H
