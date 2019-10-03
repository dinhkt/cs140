#include "pintos_thread.h"
struct station {
	int free_seat;
    int pas_wait;
    int pas_get_on;
    struct lock lock;
    struct condition seat_available;
    struct condition all_on_board;
};
void station_init(struct station *station) {
    station->free_seat = 0;
    station->pas_wait = 0;
    station->pas_get_on = 0;
    lock_init(&station->lock);
    cond_init(&station->seat_available);
    cond_init(&station->all_on_board);
}
void station_load_train(struct station *station, int count) {
    lock_acquire(&(station->lock));
    if(count&&station->pas_wait)
    {
    	station->free_seat = count;
    	cond_broadcast(&(station->seat_available),&station->lock); 
    	cond_wait(&(station->all_on_board), &(station->lock)); 
    }
    lock_release(&(station->lock));
}
void station_wait_for_train(struct station *station) {
    lock_acquire(&(station->lock)); 
    station->pas_wait++;
    while (!station->free_seat)
    {
    	cond_wait(&station->seat_available, &(station->lock));
    }
    station->pas_wait--;
    station->pas_get_on++;
    station->free_seat--;
    lock_release(&(station->lock)); 
}
void station_on_board(struct station *station) {
    lock_acquire(&(station->lock)); 
    station->pas_get_on--;
    if (!station->pas_get_on && !(station->pas_wait && station->free_seat)) 
    {
        	cond_signal(&(station->all_on_board),&station->lock);
    }
    lock_release(&(station->lock)); 
}
