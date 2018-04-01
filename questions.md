1、pthread_create()
定义传递的参数为结构体时，注意需要new
```
struct para
{

};
para* p = new para();
pthread_create(&tid, NULL, &threadfunc, p);
```
