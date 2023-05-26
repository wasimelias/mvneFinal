//
// Created by wasim on 22/05/2023.
//
#include "User.h"
#include "wet1util.h"

User::User(int user_id , bool is_vip) : userId(user_id), isVip(is_vip),group(0){
    for(int i = 0 ; i < 5 ; i++){
        views[i] = 0;
        missedViews[i] = 0;
    }
}

int User::get_userId() const{
    return userId;
}

bool User::get_isVip() const{
    return isVip;
}

int User::get_comedy_watches() const{
    return views[int(Genre::COMEDY)];
}

int User::get_drama_watches() const{
    return views[int(Genre::DRAMA)];
}

int User::get_fantasy_watches() const{
    return views[int(Genre::FANTASY)];
}

int User::get_action_watches() const{
    return views[int(Genre::ACTION)];
}

int User::get_total_watches() const{
    return views[int(Genre::NONE)];
}

int* User::get_views_array(){
    return views;
}

int* User::get_missed_views_array(){
    return missedViews;
}


void User::set_isVip(bool vip){
    isVip = vip;
}

void User::watch_comedy(){
    views[int(Genre::COMEDY)]++;
    views[int(Genre::NONE)]++;

}
void User::watch_drama(){
    views[int(Genre::DRAMA)]++;
    views[int(Genre::NONE)]++;
}

void User::watch_fantasy(){
    views[int(Genre::FANTASY)]++;
    views[int(Genre::NONE)]++;
}

void User::watch_action(){
    views[int(Genre::ACTION)]++;
    views[int(Genre::NONE)]++;
}

int User::get_group_id() const{
    return group;
}

void User::nullify(int* group_views_array){
    for(int i=0 ; i<5 ; i++){
        views[i] += group_views_array[i]-missedViews[i];
        missedViews[i] = 0;
    }
    group = 0;
}

void User::set_group(int new_group){
    group = new_group;
}
