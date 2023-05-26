#include "StreamingDBa1.h"



streaming_database::streaming_database() {

}






streaming_database::~streaming_database()
{
    users_tree.DestroyTree();
    comedy_tree.DestroyTree();
    drama_tree.DestroyTree();
    fantasy_tree.DestroyTree();
    action_tree.DestroyTree();
    none_tree_by_id.DestroyTree();
    none_tree_by_rating.DestroyTree();
    group_tree.DestroyTree();
    empty_groups_tree.DestroyTree();
}





StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
    std::shared_ptr<Movie> new_movie = nullptr;
    Node<int,Movie> *to_add_none_id = nullptr;
    Node<Movie,int>* to_add_none_rating = nullptr;
    Node<Movie,int>* to_add_genre= nullptr;

    try {
        new_movie = std::shared_ptr<Movie>(new Movie(movieId, genre, views, vipOnly));
        to_add_none_id = new Node<int, Movie>(movieId, new_movie);
        to_add_none_rating = new Node<Movie,int>(*new_movie, std::make_shared<int>(movieId));
        to_add_genre = new Node<Movie,int>(*new_movie, std::make_shared<int>(movieId));
    }
    catch(...){
        return StatusType::ALLOCATION_ERROR;
    }

    if(new_movie->check_movie() == false) {
        delete to_add_genre;
        delete to_add_none_rating;
        delete to_add_none_id;
        return StatusType::INVALID_INPUT;
    }
    if(none_tree_by_id.add_node(to_add_none_id) == false) {
        delete to_add_genre;
        delete to_add_none_rating;
        delete to_add_none_id;
        return StatusType::FAILURE;
    }

    none_tree_by_rating.add_node(to_add_none_rating);

    Genre movie_genre = new_movie->get_movieGenre();
    if(movie_genre == Genre::COMEDY){
        comedy_tree.add_node(to_add_genre);
    }
    else if(movie_genre == Genre::DRAMA){
        drama_tree.add_node(to_add_genre);
    }
    else if(movie_genre == Genre::ACTION){
        action_tree.add_node(to_add_genre);
    }
    else if(movie_genre == Genre::FANTASY){
        fantasy_tree.add_node(to_add_genre);
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
    if(movieId <= 0) return StatusType::INVALID_INPUT;
    Node<int,Movie>* toRemove = none_tree_by_id.find_node(movieId);
    if(toRemove == nullptr) return StatusType::FAILURE;
    Movie newKey = *(toRemove->get_data());

    if(none_tree_by_id.Delete(movieId) == false) return StatusType::FAILURE;

    none_tree_by_rating.Delete(newKey);
    comedy_tree.Delete(newKey);
    drama_tree.Delete(newKey);
    action_tree.Delete(newKey);
    fantasy_tree.Delete(newKey);

    return StatusType::SUCCESS;
}

StatusType streaming_database::add_user(int userId, bool isVip)
{
    if(userId <= 0) return StatusType::INVALID_INPUT;
    std::shared_ptr<User> new_user = nullptr;
    Node<int,User> *to_add = nullptr;
    try {
        new_user = std::shared_ptr<User>(new User(userId,isVip));
        to_add = new Node<int,User>(userId,new_user);
    }
    catch(...){
        return StatusType::ALLOCATION_ERROR;
    }

    if(users_tree.add_node(to_add) == false) {
        delete to_add;
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
    if(userId <= 0 ){
        return StatusType::INVALID_INPUT;
    }

    Node<int,User>* user_to_delete = users_tree.find_node(userId);

    if(user_to_delete == nullptr) {
        return StatusType::FAILURE;
    }

    int groupId = user_to_delete->get_data()->get_group_id();

    Node<int,Group>* groupNode = group_tree.find_node(groupId);
    if(groupId != 0){
        if(groupNode != nullptr) groupNode->get_data()->DeleteUserFromGroup(userId);
    }

    users_tree.Delete(userId);
    if(groupNode != nullptr) {
        int members = groupNode->get_data()->get_members().get_num_of_nodes();
        if (members <= 0) group_tree.Delete(groupId);
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::add_group(int groupId)
{
    if(groupId <= 0 ) return StatusType::INVALID_INPUT;

    std::shared_ptr<Group> group_to_add = nullptr;
    Node<int,Group>* new_group = nullptr;
    try{
        group_to_add = std::shared_ptr<Group>(new Group(groupId));
        new_group = new Node<int,Group>(groupId, group_to_add);
    }
    catch(...){
        delete new_group;
        return StatusType::ALLOCATION_ERROR;
    }

    if (empty_groups_tree.find_node(groupId) != nullptr || group_tree.find_node(groupId) != nullptr){
        delete new_group;
        return StatusType::FAILURE;
    }

    empty_groups_tree.add_node(new_group);
    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_group(int groupId)
{
    if(groupId <= 0) return StatusType::INVALID_INPUT;
    Node<int,Group>* curr_group_empty = empty_groups_tree.find_node(groupId);
    Node<int,Group>* curr_group = group_tree.find_node(groupId);
    if ((curr_group_empty == nullptr)&&(curr_group == nullptr)) return StatusType::FAILURE;

    empty_groups_tree.Delete(groupId);
    if(curr_group == nullptr) return StatusType::SUCCESS;
    removeUserFromGroup remove(*(curr_group->get_data()));
    BinaryTree<int,User>& usersTreeGroup = curr_group->get_data()->get_members();
    usersTreeGroup.inOrder(remove,usersTreeGroup.get_root());
    group_tree.Delete(groupId);

    return StatusType::SUCCESS;
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
    if(userId <= 0 || groupId <=0) return StatusType::INVALID_INPUT;
    Node<int,User>* userToFind = users_tree.find_node(userId);
    Node<int,Group>* groupToFindInEmpty = empty_groups_tree.find_node(groupId);
    Node<int,Group>* groupToFindFull = group_tree.find_node(groupId);

    if( userToFind == nullptr ||  groupToFindInEmpty == nullptr || userToFind->get_data()->get_group_id() != 0 ){
        return StatusType::FAILURE;
    }

    Node<int,User>* toAddInGroup = nullptr;
    Node<int,Group>* groupInEmptyToFull = nullptr;

    try {
        toAddInGroup = new Node<int, User>(userId, userToFind->get_data());
        if(groupToFindFull == nullptr){
            groupInEmptyToFull = new Node<int,Group>(groupId, groupToFindInEmpty->get_data());
        }

    }
    catch (...){
        return StatusType::ALLOCATION_ERROR;
    }

    if ((groupToFindFull == nullptr)&&(groupToFindInEmpty != nullptr)){
        group_tree.add_node(groupInEmptyToFull);

        groupInEmptyToFull->get_data()->UpdateStatsAddUser(toAddInGroup);

    }
    else if ((groupToFindFull != nullptr)&&(groupToFindInEmpty != nullptr)){
        groupToFindFull->get_data()->UpdateStatsAddUser(toAddInGroup);
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
    if(userId <= 0 || movieId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Node<int,User>* watcher = users_tree.find_node(userId);
    Node<int,Movie>* movie = none_tree_by_id.find_node(movieId);

    if(watcher == nullptr || movie == nullptr) return StatusType::FAILURE;
    bool isWatcherVip = watcher->get_data()->get_isVip();
    bool isMovieVip = movie->get_data()->get_isVip();
    if(isMovieVip == true && isWatcherVip == false) return StatusType::FAILURE;
    Movie oldKey = *(movie->get_data());

    movie->get_data()->watch_movie(1);
    Genre movie_genre = movie->get_data()->get_movieGenre();
    int groupIdOfWatcher = watcher->get_data()->get_group_id();
    Node<int,Group>* groupOfWatcher = group_tree.find_node(groupIdOfWatcher);
    int arrayToAddToGroupViews[5] = {0,0,0,0,1};


    if(movie_genre == Genre::COMEDY){
        watcher->get_data()->watch_comedy();
        arrayToAddToGroupViews[int(Genre::COMEDY)] = 1;
    }
    else if(movie_genre == Genre::DRAMA){
        watcher->get_data()->watch_drama();
        arrayToAddToGroupViews[int(Genre::DRAMA)] = 1;
    }
    else if(movie_genre == Genre::ACTION){
        watcher->get_data()->watch_action();
        arrayToAddToGroupViews[int(Genre::ACTION)] = 1;
    }
    else if(movie_genre == Genre::FANTASY){
        watcher->get_data()->watch_fantasy();
        arrayToAddToGroupViews[int(Genre::FANTASY)] = 1;
    }
    if (groupOfWatcher != nullptr){
        int* viewsOfGroupArr = groupOfWatcher->get_data()->get_total_views_array();
        for(int i=0 ; i<5 ; i++){
            viewsOfGroupArr[i] += arrayToAddToGroupViews[i];

        }
    }

    int data = movie->get_key();
    std::shared_ptr<Movie> key = movie->get_data();
    try {
        none_tree_by_rating.removeThenInsert(data, *key, oldKey);
        if (movie_genre == Genre::COMEDY) {
            comedy_tree.removeThenInsert(data, *key, oldKey);
        } else if (movie_genre == Genre::DRAMA) {
            drama_tree.removeThenInsert(data, *key, oldKey);
        } else if (movie_genre == Genre::FANTASY) {
            fantasy_tree.removeThenInsert(data, *key, oldKey);
        } else if (movie_genre == Genre::ACTION) {
            action_tree.removeThenInsert(data, *key, oldKey);
        }
    }
    catch(...){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
    if(movieId <= 0 || groupId <= 0) return StatusType::INVALID_INPUT;

    Node<int,Movie>* movieToFind = none_tree_by_id.find_node(movieId);
    Node<int,Group>* groupToFind = group_tree.find_node(groupId);
    if (movieToFind == nullptr || groupToFind == nullptr) return StatusType::FAILURE;

    bool movieIsVip = movieToFind->get_data()->get_isVip();
    bool groupIsVip = groupToFind->get_data()->get_isVip();
    if(movieIsVip == true && groupIsVip == false) return StatusType::FAILURE;

    int membersCount = groupToFind->get_data()->get_members().get_num_of_nodes();
    Movie oldKey = *(movieToFind->get_data());
    movieToFind->get_data()->watch_movie(membersCount);

    int data = movieToFind->get_key();
    std::shared_ptr<Movie> key = movieToFind->get_data();
    int *groupViewsArray = nullptr;
    int *groupViewsArrayTotal = nullptr;

    int arrayToAddToGroupViews[5] = {0, 0, 0, 0, 1};
    try {
        none_tree_by_rating.removeThenInsert(data, *key, oldKey);
        Genre movie_genre = movieToFind->get_data()->get_movieGenre();
        groupViewsArray = groupToFind->get_data()->get_views_array();
        groupViewsArrayTotal = groupToFind->get_data()->get_total_views_array();
        if (movie_genre == Genre::COMEDY) {
            arrayToAddToGroupViews[int(Genre::COMEDY)] = 1;
            comedy_tree.removeThenInsert(data, *key, oldKey);
        } else if (movie_genre == Genre::DRAMA) {
            arrayToAddToGroupViews[int(Genre::DRAMA)] = 1;
            drama_tree.removeThenInsert(data, *key, oldKey);
        } else if (movie_genre == Genre::ACTION) {
            arrayToAddToGroupViews[int(Genre::ACTION)] = 1;
            action_tree.removeThenInsert(data, *key, oldKey);
        } else if (movie_genre == Genre::FANTASY) {
            arrayToAddToGroupViews[int(Genre::FANTASY)] = 1;
            fantasy_tree.removeThenInsert(data, *key, oldKey);
        }
    }
    catch (...){
        return StatusType::ALLOCATION_ERROR;
    }

    for(int i=0 ; i<5 ; i++){
        groupViewsArray[i] += arrayToAddToGroupViews[i];
        groupViewsArrayTotal[i] += membersCount*arrayToAddToGroupViews[i];
    }
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
    int numOfNodes = 0;
    if(genre == Genre::NONE){
        numOfNodes = none_tree_by_id.get_num_of_nodes();
    }
    else if(genre == Genre::COMEDY){
        numOfNodes = comedy_tree.get_num_of_nodes();
    }
    else if(genre == Genre::ACTION){
        numOfNodes = action_tree.get_num_of_nodes();
    }
    else if(genre == Genre::FANTASY){
        numOfNodes = fantasy_tree.get_num_of_nodes();
    }
    else if(genre == Genre::DRAMA){
        numOfNodes = drama_tree.get_num_of_nodes();
    }

    return output_t<int>(numOfNodes);

}

StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
    if(output == nullptr) return StatusType::INVALID_INPUT;
    if (none_tree_by_id.get_num_of_nodes() == 0) return StatusType::FAILURE;
    if((genre == Genre::COMEDY) && (comedy_tree.get_num_of_nodes() == 0)){
        return StatusType::FAILURE;
    }
    else if((genre == Genre::ACTION) && (action_tree.get_num_of_nodes() == 0)){
        return StatusType::FAILURE;
    }
    else if((genre == Genre::FANTASY) && (fantasy_tree.get_num_of_nodes() == 0)){
        return StatusType::FAILURE;
    }
    else if((genre == Genre::DRAMA) && (drama_tree.get_num_of_nodes() == 0)){
        return StatusType::FAILURE;
    }
    getIdOfNode<Movie, int> getIdFunctor(output);

    if(genre == Genre::NONE){
        none_tree_by_rating.RL_inOrder<getIdOfNode<Movie,int>>(getIdFunctor ,none_tree_by_rating.get_root());
    }
    else if(genre == Genre::COMEDY){
        comedy_tree.RL_inOrder<getIdOfNode<Movie, int>>(getIdFunctor ,comedy_tree.get_root());
    }
    else if(genre == Genre::ACTION){
        action_tree.RL_inOrder<getIdOfNode<Movie, int>>(getIdFunctor ,action_tree.get_root());
    }
    else if(genre == Genre::FANTASY){
        fantasy_tree.RL_inOrder<getIdOfNode<Movie, int>>(getIdFunctor ,fantasy_tree.get_root());
    }
    else if(genre == Genre::DRAMA){
        drama_tree.RL_inOrder<getIdOfNode<Movie, int>>(getIdFunctor ,drama_tree.get_root());
    }
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
    if(userId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    Node<int,User>* userToCalcViews = users_tree.find_node(userId);
    if(userToCalcViews == nullptr) return output_t<int>(StatusType::FAILURE);
    int userGroupId = userToCalcViews->get_data()->get_group_id();
    int viewsWithGroup = 0;
    if (userGroupId > 0){
        Node<int,Group>* group = group_tree.find_node(userGroupId);
        int* groupViewsArr = group->get_data()->get_views_array();
        viewsWithGroup = groupViewsArr[int(genre)];
    }
    int* userViews = userToCalcViews->get_data()->get_views_array();
    int* userMissedViews = userToCalcViews->get_data()->get_missed_views_array();
    int totalViews = userViews[int(genre)] + viewsWithGroup - userMissedViews[int(genre)];
    return output_t<int>(totalViews);
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
    if(userId <= 0 || movieId <= 0 || rating < 0 || rating > 100) return StatusType::INVALID_INPUT;
    Node<int,User>* userToFind = users_tree.find_node(userId);
    Node<int,Movie>* MovieToFind = none_tree_by_id.find_node(movieId);
    if(userToFind == nullptr || MovieToFind == nullptr) return StatusType::FAILURE;
    if(MovieToFind->get_data()->get_isVip() == true && userToFind->get_data()->get_isVip() == false) return StatusType::FAILURE;


    Movie oldKey = *(MovieToFind->get_data());
    MovieToFind->get_data()->add_rating(rating);


    Genre movie_genre = MovieToFind->get_data()->get_movieGenre();
    std::shared_ptr<Movie> key = MovieToFind->get_data();
    int data = MovieToFind->get_key();
    try {
        none_tree_by_rating.removeThenInsert(data, *key, oldKey);

        if (movie_genre == Genre::COMEDY) {
            comedy_tree.removeThenInsert(data, *key, oldKey);
        } else if (movie_genre == Genre::DRAMA) {
            drama_tree.removeThenInsert(data, *key, oldKey);
        } else if (movie_genre == Genre::ACTION) {
            action_tree.removeThenInsert(data, *key, oldKey);
        } else if (movie_genre == Genre::FANTASY) {
            fantasy_tree.removeThenInsert(data, *key, oldKey);
        }
    }
    catch(...){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
    if(groupId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    Node<int,Group>* currGroup = group_tree.find_node(groupId);

    if (currGroup == nullptr) return output_t<int>(StatusType::FAILURE);
    Genre lovedGenre = currGroup->get_data()->getLovedGenre();
    int bestMovieId = 0;

    if(lovedGenre == Genre::COMEDY){
        if (comedy_tree.get_num_of_nodes() == 0) return output_t<int>(StatusType::FAILURE);
        bestMovieId = *(comedy_tree.getMaxNode()->get_data());
    }

    else if(lovedGenre == Genre::DRAMA){
        if (drama_tree.get_num_of_nodes() == 0) return output_t<int>(StatusType::FAILURE);
        bestMovieId = *(drama_tree.getMaxNode()->get_data());
    }

    else if(lovedGenre == Genre::ACTION){
        if (action_tree.get_num_of_nodes() == 0) return output_t<int>(StatusType::FAILURE);
        bestMovieId = *(action_tree.getMaxNode()->get_data());
    }

    else if(lovedGenre == Genre::FANTASY){
        if (fantasy_tree.get_num_of_nodes() == 0) return output_t<int>(StatusType::FAILURE);
        bestMovieId = *(fantasy_tree.getMaxNode()->get_data());
    }

    return output_t<int>(bestMovieId);

}

