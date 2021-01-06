#include "graph.h"

int ctoi(char a)
{
    if (a=='0')
    return 0;
    if (a=='1')
    return 1;
    if (a=='2')
    return 2;

}

int and[3][3] = {{0,0,0},
                 {0,1,2},
                 {0,2,2}};

int or[3][3] = {{0,1,2},
                 {1,1,1},
                 {2,1,2}};

int xor[3][3] = {{0,1,2},
                 {1,0,2},
                 {2,2,2}};

/*int xnor[3][3] = {{1,0,2},
                 {0,1,2},
                 {2,2,2}};                 
*/
int not[3] = {1,0,2};
void Simulation(NODE *g, PATTERN *p, FAULT *fau, int Max, int Total, int Tfs, FILE *fres){
    int i,j,k;
    
    //LIST *temp;
    for ( i = 0; i < Total; i++)
    {
        //int flag;
        //char *fnod;
        //char *fval;
        int o;
        //printf("yah ta chalna paryo");
        fprintf(fres, "\nFor the Pattern %s\n", p[i].piv);
        //printf("For pattern %s \n", p[i].piv);
        SimulateFfreeCircuit(g, p[i].piv, Max);
        for (j = 0; j < Tfs; j++)
        {
            int flag = 0;
            //printf("yah ta chalna paryo");
            // call routine to simulate faulty circuit
            SimulateFaultyCircuit(g, p[i].piv,fau[j].Snod,fau[j].Sval, Max);          
            for (k = 0; k <= Max; k++){               
                
                if(g[k].Po == 1){
                    if(g[k].Cval == 2 || g[k].Fval == 2){ //if primary putput don't care skip comparing
                        goto thisline;
                    }                    
                    if (g[k].Cval != g[k].Fval){                        
                        fprintf(fres,"%d/%d \t Fault Detected\n", fau[j].Snod, fau[j].Sval);
                        flag ++;
                        break;   
                    }                    
                }
                thisline:  o=0;
                //printf(" Maximum value is %d", Max);
            }
            if(flag == 0){
                fprintf(fres,"%d/%d \t Fault Not Detected\n", fau[j].Snod, fau[j].Sval);
            }             
        }
        //printf(fres, "\n");    
        
    }
    
}

void SimulateFfreeCircuit(NODE *g, char *piv, int Max)
{
    int i,j,x,y;
    LIST *temp;    
    //for(i = 0; i < Total; i++){
        int a = 0;
        for(j = 0; j<= Max; j++){
            if(g[j].Type == INPT){
                //g[j].Cval = atoi(&(p[i].pat[a]));
                x = ctoi(piv[a]);
                a++;
                //break;
                //printf("%d", x);
            }
            else if(g[j].Type == AND){
                temp = g[j].Fin;
                x = g[temp->id].Cval;
                while(temp->next != NULL){                                       
                    y = g[temp->next->id].Cval;
                    x  = and[x][y];
                    temp =temp->next;               
                }
                //printf("%d", x);            
            }
            else if(g[j].Type == OR){
                temp = g[j].Fin;
                x = g[temp->id].Cval;
                while(temp->next != NULL){                                       
                    y = g[temp->next->id].Cval;
                    x  = or[x][y];
                    temp =temp->next;               
                }
                //printf("%d", x);    
            }
            else if(g[j].Type == NOT){                                                                      
                x = g[g[j].Fin->id].Cval;
                x = not[x];
                //printf("%d", x);                       
            }
            else if(g[j].Type == NAND){
                temp = g[j].Fin;
                x = g[temp->id].Cval;                
                while(temp->next != NULL){                                       
                    y = g[temp->next->id].Cval;
                    x  = and[x][y];
                    temp = temp->next;
                    //g[j].Fin = g[j].Fin->next;               
                }
            x = not[x];
          //  g[j].Cval = x;
            //printf("%d", x);
            }
            else if(g[j].Type == NOR){
                temp = g[j].Fin;
                x = g[temp->id].Cval;
                while(temp->next != NULL){                                       
                    y = g[temp->next->id].Cval;
                    x  = or[x][y];
                    temp = temp->next;               
                }
            x = not[x];
            //printf("%d", x);
            }
            else if(g[j].Type == XOR){
                temp = g[j].Fin;
                x = g[temp->id].Cval;
                while(temp->next != NULL){                                       
                    y = g[temp->next->id].Cval;
                    x  = xor[x][y];
                    temp = temp->next;               
                }
               // printf("%d", x);    
            }
            else if(g[j].Type == XNOR){
                temp = g[j].Fin;
                x = g[temp->id].Cval;
                while(temp->next != NULL){                                       
                    y = g[temp->next->id].Cval;
                    x  = xor[x][y];
                    temp = temp->next;               
                }
                x = not[x];
                //printf("%d", x);    
            }
            else if(g[j].Type == BUFF){                                                                      
                x = g[g[j].Fin->id].Cval;
                //printf("%d", x);                                       
            }
            else if(g[j].Type == FROM){                                                                      
                x = g[g[j].Fin->id].Cval;
              //  printf("%d", x);                                       
            }
            g[j].Cval = x;
            /*if(g[j].Po == 1){
                fprintf(fres, "Cval %d\t", g[j].Cval);
            }*/ 
                      
        }
        //printf("\n");
        //fprintf(fres, "\n\n");
    //}    
}

void SimulateFaultyCircuit(NODE *g, char *piv, char *fnod, char *fval, int Max)
{
    int i,j,x,y;
    LIST *temp;
    //char hold[5];
    //char ffree[Mpo], faulty[Mpo];    
    //]for(i = 0; i < Total; i++){
        int a = 0;
        for(j = 0; j<= Max; j++){
            if(g[j].Type == INPT){
                //g[j].Cval = atoi(&(p[i].pat[a]));
                x = ctoi((piv[a]));
                a++;
                //break;
                //printf("%d", x);
                }
            else if(g[j].Type == AND){
                temp = g[j].Fin;
                x = g[temp->id].Fval;
                while(temp->next != NULL){                                       
                    y = g[temp->next->id].Fval;
                    x  = and[x][y];
                    temp = temp->next;               
                }
                //printf("%d", x);            
            }
            else if(g[j].Type == OR){
                temp = g[j].Fin;
                x = g[temp->id].Fval;
                while(temp->next != NULL){                                       
                    y = g[temp->next->id].Fval;
                    x  = or[x][y];
                    temp = temp->next;               
                }
                //printf("%d", x);    
            }
            else if(g[j].Type == NOT){                                                                      
                x = g[g[j].Fin->id].Fval;
                x = not[x];
                //printf("%d", x);                       
            }
            else if(g[j].Type == NAND){
                temp = g[j].Fin;
                x = g[temp->id].Fval;                
                while(temp->next != NULL){                                       
                    y = g[temp->next->id].Fval;
                    x  = and[x][y];
                    temp =temp->next;
                    //g[j].Fin = g[j].Fin->next;               
                }
            x = not[x];
          //  g[j].Cval = x;
            //printf("%d", x);
            }
            else if(g[j].Type == NOR){
                temp = g[j].Fin;
                x = g[temp->id].Fval;
                while(temp->next != NULL){                                       
                    y = g[temp->next->id].Fval;
                    x  = or[x][y];
                    temp = temp->next;               
                }
            x = not[x];
            //printf("%d", x);
            }
            else if(g[j].Type == XOR){
                temp = g[j].Fin;
                x = g[temp->id].Fval;
                while(temp->next != NULL){                                       
                    y = g[temp->next->id].Fval;
                    x  = xor[x][y];
                    temp = temp->next;               
                }
               // printf("%d", x);    
            }
            else if(g[j].Type == XNOR){
                temp = g[j].Fin;
                x = g[temp->id].Fval;
                while(temp->next != NULL){                                       
                    y = g[temp->next->id].Fval;
                    x  = xor[x][y];
                    temp = temp->next;               
                }
                x = not[x];
                //printf("%d", x);    
            }
            else if(g[j].Type == BUFF){                                                                      
                x = g[g[j].Fin->id].Fval;
                //printf("%d", x);                                       
            }
            else if(g[j].Type == FROM){                                                                      
                x = g[g[j].Fin->id].Fval;
              //  printf("%d", x);                                       
            }
            if(j == fnod){            
                g[j].Fval = fval;
                //printf("check");
            }    
            else
            {
                g[j].Fval = x;
                //printf("checkfree");
            }             
        }             
}

