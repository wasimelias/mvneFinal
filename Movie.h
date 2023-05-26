//
// Created by wasim on 21/05/2023.
//

#ifndef WET1_MOVIE_H
#define WET1_MOVIE_H
#include "wet1util.h"
#include <iostream>
#include <memory>

class Movie{

    int movieId ;
    Genre genre ;
    int views ;
    bool vipOnly ;
    int num_of_ratings;
    int sum_of_ratings;

public:

    Movie(int movie_id , Genre movie_genre , int views_num , bool vip_only);

    Movie(const Movie& movie) = default;

    virtual ~Movie() = default;

    int get_movie_id() const;

    Genre get_movieGenre() const;

    int get_views()const;

    bool get_isVip()const;

    double get_rating() const;

    void add_rating(int rating);

    void watch_movie(int new_views);

    bool check_movie();

    bool operator==(const Movie& movie2 ){
        return this->get_movie_id() == movie2.get_movie_id();
    }

    bool operator>(const Movie& movie2 ) {
        if(this->get_rating() > movie2.get_rating()) return true;
        if (this->get_views() > movie2.get_views() && this->get_rating() == movie2.get_rating()) return true;
        if(this->get_views() == movie2.get_views() && this->get_rating() == movie2.get_rating()) return this->get_movie_id() < movie2.get_movie_id();
        return false;
    }

    bool operator<(const Movie& movie2 ){
        return (!(*this == movie2) && !(*this > movie2));
    }
};

/*
bool operator==(const Movie& movie1 , const Movie& movie2 ){
    return movie1.get_movie_id() == movie2.get_movie_id();
}


bool operator<(const Movie& movie1 , const Movie& movie2 ){
    if(movie1.get_rating() < movie2.get_rating()) return true;
    else if (movie1.get_views() < movie2.get_views()) return true;
    return movie1.get_movie_id() > movie2.get_movie_id();
}

bool operator>(const Movie& movie1 , const Movie& movie2 ){
    if(movie1.get_rating() > movie2.get_rating()) return true;
    else if (movie1.get_views() > movie2.get_views()) return true;
    return movie1.get_movie_id() < movie2.get_movie_id();
}
*/

#endif //WET1_MOVIE_H
