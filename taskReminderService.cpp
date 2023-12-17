#include<fstream>
#include<thread>
#include<mutex>
#include<windows.h>
#include<mmsystem.h>
#include<time.h>
#include<iostream>
#include<Windows.h>
using namespace std;
typedef struct work
{
    string s;
    string date;
    string time;
    work* next=NULL;
    int checked=0;
}work;

work* head=NULL;
mutex m;


string* getDateTime ()
{
static string dt[2];
time_t rawtime;
struct tm * timeinfo;
char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,sizeof(buffer),"%d-%m-%Y",timeinfo);
  std::string str(buffer);
strftime(buffer,sizeof(buffer),"%H:%M",timeinfo);
  std::string str2(buffer);
  dt[0]=str;
  dt[1]=str2;
return dt;
}


void alarm()
{
    while(1)
    {
        m.lock();
        string* dt=getDateTime();
        if(head!=NULL)
        {
                work* temp=head;
                while(temp!=NULL){
                if(temp->date==dt[0]&&temp->time==dt[1])
                {
                    if(temp->checked==0){
                    temp->checked=1;
                    PlaySound(TEXT("C:\\Users\\TACHLAND\\Desktop\\ac.wav"),NULL,SND_ASYNC);
                     this_thread::sleep_for (chrono::seconds(4));
                    break;
                    }
                }
                else
                {
                    temp->checked=0;
                }
                    temp=temp->next;
            }
        }

        m.unlock();

    }
}



void updateList()
{
    while(1)
    {
       m.lock();
        int flag=0;
        string task="";
        string* dt=getDateTime();
        work* it=head;
        while(it!=NULL)
        {
             if(it->date==dt[0]&&it->time==dt[1] && it->checked==1)
                {
                  flag=1;
                  task=it->s;
                  break;
                }
               it= it->next;
        }
       ifstream f;
    work* temp=new work();
    work* temp3=NULL;
    work* temp2=NULL;
    work* newHead=NULL;
    while(1){
            try{
    f.open("C:\\Users\\TACHLAND\\Desktop\\todolist.txt");
    if(!f){
            throw 10;
    }
    else{

    string* dt=getDateTime();
    int ck=0;
    while(getline(f,temp->s))
    {
        ck=1;
        getline(f,temp->date);
         getline(f,temp->time);
         if(flag==1 && temp->date==dt[0] && temp->time==dt[1] && temp->s==task)
         {
             temp->checked=1;
         }
         else{
                temp->checked=0;
         }
         temp2=new work();
         temp2->s=temp->s;
         temp2->date=temp->date;
         temp2->time=temp->time;
         temp2->checked=temp->checked;
         if(newHead==NULL)
            newHead=temp2;
         else{
             temp3=newHead;
             while(temp3->next!=NULL)
                temp3=temp3->next;
             temp3->next=temp2;
         }

    }
    head=newHead;
    f.close();
    break;
    }
            }
            catch(...)
            {
                Sleep(10);
            }
    }

      m.unlock();
  std::this_thread::sleep_for (std::chrono::seconds(4));

    }

}


int main()
{
thread t1(alarm);
thread t2(updateList);
t1.join();
t2.join();
return 0;
}

