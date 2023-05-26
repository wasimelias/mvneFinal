//
// Created by wasim on 21/05/2023.
//



#ifndef WET1_USER_H
#define WET1_USER_H


class User{
    int userId;
    bool isVip;
    int views[5];
    int missedViews[5];
    int group;

public:

    User(int user_id , bool is_vip);                                                                                      ////null group

    virtual ~User() = default;                                                                                              /////

    int get_userId() const;

    bool get_isVip() const;

    int get_comedy_watches() const;

    int get_drama_watches() const;

    int get_fantasy_watches() const;

    int get_action_watches() const;

    int get_total_watches() const;

    int* get_views_array();

    int* get_missed_views_array();

    int get_group_id() const;

    void set_isVip(bool vip);

    void watch_comedy();

    void watch_drama();

     void watch_fantasy();

    void watch_action();

    void nullify(int* group_views_array);

    void set_group(int new_group);

};



#endif // WET1_USER_H
