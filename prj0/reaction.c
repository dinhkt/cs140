#include "pintos_thread.h"
void make_water();
struct reaction {
	int n_H;
	int n_O;
	struct lock reaction_lock;
	struct condition gotO;
	struct condition gotH;
};
void reaction_init(struct reaction *reaction) {	
	reaction->n_H=0;
	reaction->n_O=0;
	lock_init(&reaction->reaction_lock);
	cond_init(&reaction->gotH);
	cond_init(&reaction->gotO);
}
void reaction_h(struct reaction *reaction) {
	lock_acquire(&reaction->reaction_lock);
	reaction->n_H++;
	if(reaction->n_O>1)
	{	
	if(reaction->n_H>=2&&reaction->n_O>=1)
	{
		make_water();
		reaction->n_H-=2;
		reaction->n_O--;
		cond_signal(&reaction->gotO,&reaction->reaction_lock);
		cond_wait(&reaction->gotH,&reaction->reaction_lock);
	}
	else 
		cond_wait(&reaction->gotH,&reaction->reaction_lock);
	}
	lock_release(&reaction->reaction_lock);	
}
void reaction_o(struct reaction *reaction) {
	lock_acquire(&reaction->reaction_lock);
	reaction->n_O++;
	cond_wait(&reaction->gotO,&reaction->reaction_lock);
	cond_signal(&reaction->gotH,&reaction->reaction_lock);
	cond_signal(&reaction->gotH,&reaction->reaction_lock);
	lock_release(&reaction->reaction_lock);
}
