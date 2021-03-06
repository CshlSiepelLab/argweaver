#ifndef ARGWEAVER_THREAD_H
#define ARGWEAVER_THREAD_H

#include "local_tree.h"
#include "states.h"


namespace argweaver {

// assert that a thread is compatiable with an ARG
bool assert_trees_thread(LocalTrees *trees, int *thread_path, int ntimes);

// adding/removing branches from a local tree
void add_tree_branch(LocalTree *tree, int node, int time, int pop_path=0);
void remove_tree_branch(LocalTree *tree, int remove_leaf, const ArgModel *model,
                        int *displace=NULL);

// update an SPR and mapping after adding a new branch
void add_spr_branch(const LocalTree *tree, const LocalTree *last_tree,
                    State state, State last_state,
                    Spr *spr, int *mapping,
                    int newleaf, int displaced, int newcoal);
void add_spr_branch(const LocalTree *tree, const LocalTree *last_tree,
                    State state, State last_state,
                    Spr *spr, int *mapping,
                    int subtree_root, int last_subtree_root);


// add a thread to an ARG
void add_arg_thread(LocalTrees *trees, const StatesModel &states_model,
                    int ntimes, int *thread_path, int seqid,
                    vector<int> &recomb_pos, vector<Spr> &recombs,
                    const PopulationTree *pop_tree=NULL);

// remove a thread from an ARG
 void remove_arg_thread(LocalTrees *trees, int remove_seqid,
                        const ArgModel *model);


// Add a branch to a partial ARG
void add_arg_thread_path(LocalTrees *trees, const StatesModel &states_model,
                         int ntimes, const int *thread_path,
                         vector<int> &recomb_pos, vector<Spr> &recombs,
                         const PopulationTree *pop_tree=NULL);
// Removes a thread path from an ARG and returns a partial ARG
void remove_arg_thread_path(LocalTrees *trees, const int *removal_path,
                            int maxtime, const PopulationTree *pop_tree=NULL,
                            int *original_thread=NULL);


//=============================================================================
// removal paths


class RemovalPaths
{
public:
    RemovalPaths(const LocalTrees *trees) :
        counts(NULL),
        backptrs(NULL)
    {
        alloc(trees);
    }

    RemovalPaths(int nnodes, int ntrees) :
        counts(NULL),
        backptrs(NULL)
    {
        alloc(nnodes, ntrees);
    }

    ~RemovalPaths()
    {
        clear();
    }

    typedef int next_row[2];

    void alloc(const LocalTrees *trees)
    {
        alloc(trees->nnodes, trees->get_num_trees());
    }

    void alloc(int _nnodes, int _ntrees)
    {
        clear();

        nnodes = _nnodes;
        ntrees = _ntrees;

        // allocate path counts and traceback tables
        counts = new_matrix<double>(ntrees, nnodes);
        backptrs = new_matrix<next_row>(ntrees, nnodes);
    }

    void clear()
    {
        if (counts) {
            delete_matrix<double>(counts, ntrees);
            counts = NULL;
        }
        if (backptrs) {
            delete_matrix<next_row>(backptrs, ntrees);
            backptrs = NULL;
        }
    }


    int nnodes;
    int ntrees;
    double **counts;
    next_row **backptrs;
};


// sample removal paths
void sample_arg_removal_path(const LocalTrees *trees, int node, int *path);
void sample_arg_removal_path(
    const LocalTrees *trees, int node, int pos, int *path,
    double prob_switch=.1);
void sample_arg_removal_leaf_path(const LocalTrees *trees, int node, int *path);

void sample_arg_removal_path_recomb(
    const LocalTrees *trees, double recomb_preference, int *path);

// count number of removal paths
void count_arg_removal_paths(const LocalTrees *trees,
                             RemovalPaths &removal_paths);

// count total number of removal paths
double count_total_arg_removal_paths(const RemovalPaths &removal_paths);

// sample a removal path uniformly from all paths and return total path count
 double sample_arg_removal_path_uniform(const LocalTrees *trees, int *path);

// return the removal path relating to a particular haplotypes ancestry
// during the time span between time_interval and time_interval+1
// should be done independently.
// break_coords is set to corresponding coordinates at the start of these breaks
 void get_arg_removal_path_by_ind_and_time(const LocalTrees *trees,
                                           int time_interval,
                                           int hap,
                                           int *path,
                                           vector<int> &break_coords,
                                           bool do_first=true);

 // count total number of removal paths
double count_total_arg_removal_paths(const LocalTrees *trees);


/*
// sample a removal path using the branch cut method
void sample_arg_removal_path_cut(const LocalTrees *trees, int ntimes,
                                 int *path, int *cuttime,
                                 int *region_start, int *region_end,
                                 int window_start=-1, int window_end=-1);
*/

void get_next_removal_nodes(const LocalTree *tree1, const LocalTree *tree2,
                            const Spr &spr2, const int *mapping2,
                            int node, int next_nodes[2], int recoal=-1);

void get_prev_removal_nodes(const LocalTree *tree1, const LocalTree *tree2,
                            const Spr &spr2, const int *mapping2,
                            int node, int prev_nodes[2], int *inv_mapping=NULL);

} // namespace argweaver

#endif //ARGWEAVER_THREAD_H
