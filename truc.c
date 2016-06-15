#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "liste.h"
#include "truc.h"

Un_truc *creer_truc(Une_coord coord,Ttype type,Tdata data,double uv){
	Un_truc *t;
	t=malloc(sizeof(Un_truc));
	if(t==NULL){
		fprintf(stderr,"Erreur d'allocation de memoire dans creer_truc \n");
		return NULL;
	}
	t->coord=coord;
	t->type=type;
	t->data=data;
	t->user_val=uv;
	return t;
}


void detruire_truc(Un_truc *truc){
	switch(truc->type){
	case STA:
		free(truc->data.sta.nom);
		free(truc->data.sta.tab_con);
	case CON:
	break;
	default:
		fprintf(stderr, "erreur.type inconnue. \n");
		}
	free(truc);
}
