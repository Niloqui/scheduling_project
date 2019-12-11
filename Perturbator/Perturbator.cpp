#include "Perturbator.hpp"
#include "Utility2.hpp"
#include "Solution.hpp"


/*
*****************************ATTENTION*****************************
 For debuggin purposes, uncomment only the
 "print_i_v" or "printv" functions that are under the comment:
 "// print for debugging"
*****************************ATTENTION*****************************
 */

Perturbator::Perturbator(){

}


void Perturbator::generate_childs(std::vector<int> previous,int small = 15,int medium=30, int large=45){
    this->previous = previous;
    this->vect_size = previous.size();
    for(int i = 0; i < this->vect_size; i++)
        this->previous_indexes.push_back(i);
    // print for debugging
    //print_i_v(this->previous_indexes,this->previous,"vector 'previous '");

    this->shuffled_indexes = shuffle_v(this->previous_indexes);
    for(int i = 0; i < this->vect_size; i++){
        int rand_index = shuffled_indexes[i];
        this->shuffled.push_back(this->previous[rand_index]);
    }
    // print for debugging
    //print_i_v(this->shuffled_indexes,this->shuffled,"vector 'shuffled'");
    this->s_perturbated=this->perturbate(ceil(this->vect_size* (float)small/100));
    //this->m_perturbated=this->perturbate(ceil(this->vect_size* (float)medium/100));
    //this->l_perturbated=this->perturbate(ceil(this->vect_size* (float)large/100));
}

std::vector<int> Perturbator::perturbate(int size) {
    std::vector<int> sub_indexes_vect;
    std::vector<int> sub_vect;
    for (int i = 0; i < size; i++){
        sub_indexes_vect.push_back(this->shuffled_indexes[i]);
        sub_vect.push_back(this->shuffled[i]);
    }
    //printv(sub_indexes_vect,"vector 'sub_indexes_vect'");
    // print for debugging
    //print_i_v(sub_indexes_vect,sub_vect,"vector 'sub_vect'");
    std::vector<int> sub_shuffled_indexes = shuffle_v(sub_indexes_vect);
    std::vector<int> sub_shuffled_vect;
    for(int i = 0; i < size; i++){
        //int rand_index = sub_shuffled_indexes[i];
        sub_shuffled_vect.push_back(sub_vect[i]);
    }
    //printv(sub_shuffled_indexes,"vector 'sub_shuffled_indexes_vect'");
    // print for debugging
    //print_i_v(sub_shuffled_indexes,sub_shuffled_vect,"vector 'sub_shuffled_vect'");

    //recomposing original vector with shuffled items;
    std::vector<int> recomposed = this->previous;
    for(int i = 0; i < size; i++){
        int value = sub_shuffled_vect[i];
        int index = sub_shuffled_indexes[i];
        recomposed[index] = value;
    }
    // print for debugging
    //printv(recomposed,"vector 'recomposed'");
    //std::vector<int> sol = this->generate_feasible_solution(recomposed);
    return recomposed;

}

const std::vector<int> &Perturbator::getSPerturbated() const {
    return s_perturbated;
}

const std::vector<int> &Perturbator::getMPerturbated() const {
    return m_perturbated;
}

const std::vector<int> &Perturbator::getLPerturbated() const {
    return l_perturbated;
}

