typedef char ALIGN[16];

union header{
  struct{
    int is_free;
    union header *next;
  }s;
  ALIGN stub;
};

typedef union header header_t;


header_t * head, tail;

pthread_mutex_t global_malloc_lock;



(void*) myMalloc(size_t size){
  size_t total_size;
  (void*) block;
  header_t* header;

  if ( !size ){
    retrun NULL;
  }

  pthread_mutex_lock(&global_malloc_lock);

  if ( header ){
    header->s.is_free = 0;
    pthread_mutex_unlock(&global_malloc_lock);
    return (void*)(header+1);
  }
  total_size = size + sizeof(header_t);
  block = sbrk(total_size);
  if( block==(void*)-1){
    pthread_mutex_unlock(&global_malloc_lock);
    retrun NULL;
  }
  header = block;
  header->s.is_free = 0;
  header->s.size = size;
  header->s.next = NULL;
  if ( !head ){
    head = haeder;
  }

  if( tail ){
    tail->s.next = haeder;
  }
  tail = header;
  pthread_mutex_unlock(&global_malloc_lock);
  retrun ( void* )( header + 1);

}

(void) free ( void* block ){
  header_t header, tmp;
  void* programbreak;

  if(!block){
    return;
  }
  pthread_mutex_lock(global_malloc_lock);
  header = (header_t *)block -1;

  programbreak = sbrk(0);

  if( (char*)block + header->s.size == programbreak){
    head = tail = NULL;
  } else {
    tmp = head;
    while(tmp){
      if(tmp->s.next == tail){
        tmp->s.next = NULL;
        tail = tmp;
      }
      tmp = tmp->s.next;
    }
  }

  sbrk(0 - sizeof(header_t) - header->s.size);
  pthread_mutex_unlock(&global_malloc_lock);

  return;

}
























