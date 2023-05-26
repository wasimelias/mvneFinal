//
// Created by wasim on 22/05/2023.
//
#include "Movie.h"

Movie::Movie(int movie_id , Genre movie_genre , int views_num , bool vip_only) : movieId(movie_id), genre(movie_genre),
                                        views(views_num), vipOnly(vip_only),num_of_ratings(0) ,sum_of_ratings(0){}

int Movie::get_movie_id() const{
    return movieId;
}

Genre Movie::get_movieGenre() const{
    return genre;
}

int Movie::get_views() const{
    return views;
}

bool Movie::get_isVip() const{
    return vipOnly;
}

double Movie::get_rating() const{
    if(num_of_ratings == 0) return 0.0;
    return (double)(sum_of_ratings) / num_of_ratings ;
}

void Movie::add_rating(int rating){
    sum_of_ratings += rating;
    num_of_ratings++;
}

void Movie::watch_movie(int new_views){
    views += new_views;
}

bool Movie::check_movie(){
    if(movieId <= 0 || genre == Genre::NONE || views < 0)
        return false;
    return true;
}


