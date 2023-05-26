//
// Created by wasim on 22/05/2023.
//

#include "Group.h"

Group::Group(int id) : groupId(id), vip_count(0){
    for(int i = 0 ; i < 5 ; i++){
        views[i] = 0;
        totalViews[i] = 0;
    }
}

Group::~Group(){
    members.DestroyTree();
}

int Group::get_group_id() const{
    return groupId;
}

bool Group::get_isVip() const{
    return vip_count != 0;
}

int Group::get_comedy_watches() const{
    return views[int(Genre::COMEDY)];
}

int Group::get_drama_watches() const{
    return views[int(Genre::DRAMA)];
}

int Group::get_fantasy_watches() const{
    return views[int(Genre::FANTASY)];
}

int Group::get_action_watches() const{
    return views[int(Genre::ACTION)];
}

int Group::get_total_watches() const{
    return views[int(Genre::NONE)];
}

BinaryTree<int,User>& Group::get_members() {
    return members;
}

int* Group::get_views_array(){
    return views;
}

int* Group::get_total_views_array(){
    return totalViews;
}


void Group::DeleteUserFromGroup(int userId){                                                                            ////////////////
    bool is_vip = members.find_node(userId)->get_data()->get_isVip();
    if (is_vip){
        vip_count--;
    }
    Node<int,User>* userInGroup = members.find_node(userId);
    int* userViews = userInGroup->get_data()->get_views_array();
    int* userMissedViews = userInGroup->get_data()->get_missed_views_array();

    for(int i=0 ; i<5 ;i++) {
        totalViews[i] = totalViews[i] - (userViews[i] + views[i] - userMissedViews[i]);
    }
    members.Delete(userId);
}

void Group::UpdateStatsAddUser(Node<int,User>* userToAdd){
    userToAdd->get_data()->set_group(groupId);
    int* userViews = userToAdd->get_data()->get_views_array();
    int* userMissedViews = userToAdd->get_data()->get_missed_views_array();
    if (userToAdd->get_data()->get_isVip()){
        vip_count++;
    }
    for(int i = 0 ; i < 5 ; i++){
        totalViews[i] += userViews[i];
        userMissedViews[i] = views[i];
    }
    members.add_node(userToAdd);
}


Genre Group::getLovedGenre() {
    int maxViews = 0;
    for(int i=0 ; i<4 ; i++){
        if(totalViews[i] > maxViews) maxViews = totalViews[i];
    }
    if(maxViews == totalViews[0]) return Genre::COMEDY;
    if(maxViews == totalViews[1]) return Genre::DRAMA;
    if(maxViews == totalViews[2]) return Genre::ACTION;
    if(maxViews == totalViews[3]) return Genre::FANTASY;
    return Genre::NONE;
}
