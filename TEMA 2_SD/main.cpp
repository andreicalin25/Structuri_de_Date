#include <iostream>
#include <climits>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define MIN INT_MIN                 //in MIN (si MAX) introduc cea mai mica (respectiv cea mai mare) valoare din C++
#define MAX INT_MAX
int max_lvl=0;                      //initial max_lvl este 0, deoarece nu am inserat elemente in lista


struct nod {
    int data;
    int lvl = 0;
    struct nod* next = NULL;                //un nod are implicit toate legaturile NULL
    struct nod* prev = NULL;                //in acest fel ma asigur ca nodurile "marginale" sunt legate doar de
    struct nod* up = NULL;                  //celelalte noduri din lista
    struct nod* down = NULL;
};
typedef struct nod *list;


void initializare_lista(list &head, list &tail);
int nr_lvl();
void generare_lvl(list &head, list &tail, int lvl);
void insert(list &head, list &tail, int x);
bool cautare(list &head, int x);
void remove(list &head, list &tail, int x);
void reducere_lvl(list &head, list &tail);
int succesor(list &head, int x);
int predecesor(list &head, int x);
void interval(list &head, int x, int y);
void print(list &head);
void afisare_elemente(list &head);


int main()
{

    srand ( time(NULL) );           //la acest pas ma asigur ca nivelele din skiplist vor fi diferite de fiecare data
    bool okay = true;

    list head = new nod;
    list tail = new nod;
    initializare_lista(head, tail);

    for(int i=0; i<=200; i++)
        cout<<"_";
    cout<<"\nAlegeti una dintre urmatoarele comenzi:\n"
          "1 X: inserati numarul X in multime\n"
          "2 X: stergeti numarul X din multime (daca acesta exista)\n"
          "3 X: afisati 1 daca numarul X este in multime, alftel afisati 0\n"
          "4 X: afisati cel mai mare numar Y, mai mic sau egal cu X, sau -1 daca nu exista numere mai mici ca X in lista\n"
          "5 X: afisati cel mai mic numar Y, mai mare sau egal cu X, sau -1 daca nu exista numere mai mari ca X in lista\n"
          "6 X Y: afisati in ordine sortata, toate numerele Z, unde X ≤ Z ≤ Y\n"
          "7 : afisati toate elementele din lista sortate\n"
          "8 : afisati tot skip list-ul pe nivele\n"
          "9 : terminati executia programului\n";


    while (okay)
    {
        int operatie, x, y;

        cin>>operatie;

        switch(operatie)

        {

            case 1:

                cin>>x;
                insert(head, tail, x);
                cout<<"Valoarea "<<x<<" inserata cu succes\n";
                break;

            case 2:

                cin>>x;

                if(!cautare(head, x))
                {
                    cout<<"Elementul nu se afla in lista\n";
                    break;
                }

                remove(head, tail, x);
                cout<<"Valoarea "<<x<<" stearsa cu succes\n";

                break;

            case 3:

                cin>>x;
                cout<<cautare(head, x)<<endl;
                break;

            case 4:

                cin>>x;
                if(predecesor(head, x) == MIN)
                    cout<<"-1\n";
                else
                    cout<<predecesor(head, x)<<endl;
                break;

            case 5:

                cin>>x;
                if(succesor(head, x) == MAX)
                    cout<<"-1\n";
                else
                    cout<<succesor(head, x)<<endl;
                break;

            case 6:

                cin>>x>>y;
                interval(head, x, y);
                break;

            case 7:

                afisare_elemente(head);
                cout<<endl;
                break;

            case 8:

                print(head);
                break;

            case 9:

                okay = false;
                break;

            default:

                cout<<"nu ati introdus comanda corecta"<<endl;

        }
    }
}


void initializare_lista(list &head, list &tail) {
    head->data = MIN;                               //pe explicatiile gasite pe net scria ca prima si ultima valoare din
    tail->data = MAX;                               //skip list trebuie sa fie +infinit si -infinit
    head->next = tail;                              //nu stiu cum se scriu valorile de infinit in C++, asa ca m-am
    head->prev = NULL;                              //folosit de valorile maxime pe care le memoreaza un INT
    tail->prev = head;
    tail->next = NULL;                              ///lista este initial   MIN <--> MAX
}                                                   //voi efectua toate operatiile din skip list intre cele doua extreme


void insert(list &head, list &tail, int x)
{
    int lvl = nr_lvl();                     //vad pe cate nivele trebuie sa inserez elementul

    //cout<<"pun valoarea "<<x<<" pe "<<lvl+1<<" nivele"<<endl;

    if (lvl > max_lvl) {                    //daca nu am destule nivele disponibile, mai creez unele noi
        generare_lvl(head, tail, lvl);
        max_lvl = lvl;                      //reinnoiesc valoarea lui max_lvl cu noul maxim
    }

    list a = head;                          //de a ma folosesc pentru a insera nodul de "sus in jos" in skip list;
    while(a->lvl != lvl)                    //cat timp nivelul lui a este mai mare decat nivelul nodului pe care il
        a = a->down;                        //vreau eu, cobor in skiplist

                                            ///incep inserarea propriu-zisa

    list aux = tail;                        //auxiliar de care ma folosesc pentru a face legaturile pe "verticala"
                                            //il initializez cu tail, puteam sa-l initializez si cu NULL, nu conteaza

    for(int i=lvl; i>=0; i--, a = a->down)
    {
        list before = a;
        while(before->next->data < x)       //pregatesc pozitia pe care trebuie inserat nodul nou
            before = before->next;          ///in before retin elementul dinaintea nodului meu

        list after = before->next;          ///in after retin elementul care va fi dupa nodul meu

        list p = new nod;                   //la fiecare pas instantiez un nou nod pentru fiecare rand al skip list-ului
        p->data = x;

        before->next = p;
        p->prev = before;
                                            //fac legaturile cu before si after
        after->prev = p;
        p->next = after;

        p->lvl = i;                         //i imi spune la fiecare pas pe ce nivel al skip list-ului ma aflu

        if(aux != tail){                    //daca am inserat deja elementul pe cel mai inalt nivel, am grija ca
            aux->down = p;                  //urmatoarele de mai jos sa fie legate cu cel imediat superior
            p->up = aux;
        }

        aux = p;
    }
}


int nr_lvl()                        //aici calculez numarul de nivele pentru noul numar din lista, simuland "flip a coin"
{
    int nr=0;                       //pornesc cu numarul de nivele initial 0
    while(true)
    {   int i=rand()%2;             //i primeste random valoarea 0 sau 1
        if(i==0) nr++;              //daca i==0, numarul de nivele creste
        else return nr;}            //atunci cand dau de un 1, returnez numarul de nivele curent
}


void generare_lvl(list &head, list &tail, int lvl)
{
    for(int i=lvl; i > max_lvl; i--)            //in i retin nielul curent
    {
        list m = new nod;                    //creez cate o instanta ale nodurilor head si tail pentru fiecare nou nivel
        list n = new nod;

        head->up = m;                        //m si n sunt la momentul crearii mai sus cu un nivel fata de head si tail
        tail->up = n;

        m->down = head;                      //fac legaturile necesare
        n->down = tail;

        m->data = head->data;
        n->data = tail->data;

        m->next = n;
        n->prev = m;

        m->lvl = lvl - i + max_lvl + 1;         //formula calculata cu pix si hartie pentru a ma asigura ca pe coloane
        n->lvl = lvl - i + max_lvl + 1;         //fiecare element sa retina nivelul pe care se afla

        head = m;                           /// ma asigur ca head (si tail) sa indece mereu varfurile din stanga sus
        tail = n;                           /// (respectiv dreapta sus) ale listei !!!!!!!!!!!!
    }
}


bool cautare(list &head, int x)
{
    list p = head;                                 //ma folosesc de p pentru parcurgerea listei

    while(true)                                    //parcurg lista
    {
        if(p->data < x)                              //daca nodul in care ma aflu este prea mic, merg in dreapta sau in jos;
            if(p->next->data > x)                    //daca nodul din dreapta are o valoare prea mare, merg in jos, dar
                if(p->down != NULL)                  //merg in jos doar daca am unde, ca daca sunt la baza listei,
                    p = p->down;                     //inseamna ca valoarea nu se afla in lista
                else
                    return false;
            else
                p=p->next;                           //daca nodul din dreapta nu este prea mare, merg in el

        if(p->data == x)                             //daca am gasit nodul returnez true
            return true;

        if(p->next == NULL)                          //daca am ajuns la capatul din dreapta al listei returnez false
            return false;

        if(p->data > x)                              //daca valoarea in care am ajuns este prea mare, returnez false
            return false;
    }
}


void remove(list &head, list &tail, int x)
{
    list p = head;                                 //ma folosesc de p pentru parcurgerea listei

    while(p->data != x)                                    //parcurg lista
    {
        if(p->next->data > x)
            p = p->down;
        else
            p = p->next;
    }

    while(p!=NULL)
    {
        list before = p->prev;                      //refac legaturile pentru a "sari" peste elementul de eliminat
        list after = p->next;
        before->next = after;
        after->prev = before;

        list crt = p;                               //pregatesc de eliminare elementul de pe randul curent
        p = p->down;                                //pregatesc pasul urmator
        delete crt;                                 //elimin elementul de pe nivelul curent

    }

    reducere_lvl(head, tail);                  //daca elementul sters era singurul de pe mai multe linii, trebuie
}                                                    //sa sterg liniile in plus


void reducere_lvl(list &head, list &tail)
{
    while(head->next == tail && head->lvl != 0)         ///cat timp am nivele vide, acestea trebuiesc sterse
    {
        list a = head;                              //le pregatesc de stergere
        list b = tail;
        head = head->down;                          //cobor in lista cu head si tail
        tail = tail->down;
        delete a;
        delete b;                                   //le sterg, avand grija sa scad in acelasi timp si max_lvl
        max_lvl--;
    }
}


int succesor(list &head, int x)
{
    list p = head;

    while(p->lvl!=0 && p->data<=x)              //cat timp nu sunt pe ultimul nivel si ma aflu pe valori mai mici decat x
    {
        if(p->next->data > x)                   //merg in jos sau in drepata in skip list
            p = p->down;
        else
            p = p->next;
    }

    while(p->data < x)                      //cand am ajuns pe cel mai de jos nivel, merg in dreapta pana la o valoare
        p = p->next;                        //mai mare sau egala cu x


    if(p->data == x)                        //daca ma aflu pe o valoare egala cu x, o afisez pe urmatoarea din dreapta
        return p->next->data;
    else
        return p->data;                     //daca ma aflu pe o valoare mai mare ca x, inseamna ca x nu se afla in lista
                                            //si valoarea in care am ajuns este cea imediat mai mare ca x
}


int predecesor(list &head, int x)           //algoritmul este identic cu cel de la superior
{                                           //singura diferenta este ca ma asigur sa afisez elementele dinaintea x-ului
    list p = head;

    while(p->lvl!=0 && p->data<=x)
    {
        if(p->next->data > x)
            p = p->down;
        else
            p = p->next;
    }

    while(p->data < x)
        p = p->next;

    p = p->prev;               //acest pas este necesar pentru ca eu pot ajunge in valori strict mai mari ca x

    if(p->data == x)
        return p->prev->data;
    else
        return p->data;
}


void interval(list &head, int x, int y)     //acelasi algortim ca la succesor, doar ca aici afisez mai multe valori
{                                           //mai exact, toate cele mai mici sau egale cu y
    list p = head;

    while(p->lvl!=0 && p->data<=x)
    {
        if(p->next->data > x)
            p = p->down;
        else
            p = p->next;
    }

    while(p->data < x)
        p = p->next;

    while(p->data <= y)
    {
        cout<<p->data<<" ";
        p = p->next;
    }
    cout<<endl;
}


void print(list &head)
{
    list p = head;

    if(p->lvl == 0 && p->next->data == MAX)                //daca am un singur nivel (de baza) si nu am alte elemente
        cout<<"lista este vida";                           //pe langa MIN si MAX, lista este vida
    else
        for(p; p!=NULL; p = p->down)
        {
            list q;
            for(q=p; q!=NULL; q=q->next)

                if(q->data != MIN && q->data!=MAX)   ///aceasta linie se poate comenta, caz in care vor fi afisate si limitele laterale ale skip list-ului

                    cout<<q->data<<" ";

            cout<<endl;
        }
}


void afisare_elemente(list &head)
{
    list p = head;

    while(p->lvl != 0)                  //merg pana la cel mai de jos nivel al listei
    {
        p = p->down;
    }
    p=p->next;                          //sar limita din stanga

    if(p->next == NULL)
        cout<<"lista este vida";       //daca ma aflu deja in limita din dreapta, lista este vida
    else
        while(p->next != NULL)          //pana cand nu ajung la limita dreapta, afisez valori
        {
            cout<<p->data<<" ";
            p = p->next;
        }
}