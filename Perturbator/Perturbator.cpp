#include "Perturbator.hpp"

Perturbator::Perturbator(){

}

std::vector<int> shuffle_vect(std::vector<int> vect);

void Perturbator::generate_childs(std::vector<int> previous,int small = 15,int medium=30, int large=45){
    this->previous = previous;
    this->vect_size = previous.size();
    for(int i = 0; i < this->vect_size; i++){
        this->previous_indexes.push_back(i);
    }
    this->shuffled_indexes = shuffle_vect(this->previous_indexes);
    for(int i = 0; i < this->vect_size; i++){
        int rand_index = shuffled_indexes[i];
        this->shuffled.push_back(this->previous[rand_index]);
    }
    this->s_perturbated=this->perturbate(ceil(this->vect_size*small/100));
    this->m_perturbated=this->perturbate(ceil(this->vect_size*medium/100));
    this->l_perturbated=this->perturbate(ceil(this->vect_size*large/100));
}
std::vector<int> Perturbator::perturbate(int size){
    std::vector<int> sub_indexes_vect;
    std::vector<int> sub_vect;
    for(int i = 0; i < size; i++){
        sub_indexes_vect.push_back(i);
        sub_vect.push_back(this->shuffled[i]);
    }
    
    std::vector<int> sub_shuffled_indexes = shuffle_vect(sub_indexes_vect);
    std::vector<int> sub_shuffled_vect;
    for(int i = 0; i < size; i++){
        int rand_index = sub_shuffled_indexes[i];
        sub_shuffled_vect.push_back(sub_indexes_vect[rand_index]);
    }
    

    //recomposing original vector with shuffled items;
    std::vector<int> recomposed = this->previous;
    for(int i = 0; i < size; i++){
        int value = sub_shuffled_vect[i];
        int index = sub_shuffled_indexes[i];
        recomposed[index] = value;
    }
    return recomposed;

}



/*std::vector<int> shuffle_vect(std::vector<int> vect){
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();

    shuffle (vect.begin(), vect.end(), std::default_random_engine(seed));
    
    return vect;
}*/
