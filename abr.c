#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "abr.h"
#include "liste.h"

Un_nabr *creer_nabr(Un_truc *truc){
	Un_nabr *n;
	n=malloc(sizeof(Un_nabr));
	if(n==NULL){
		fprintf(stderr,"Erreur d'allocation de memoire dans Un_nabr \n");
		return NULL;
	}
	n -> truc = truc;
	n -> g = NULL;
	n -> d = NULL;
	
	return n;
}


Un_nabr *inserer_abr(Un_nabr *abr , Un_nabr *n){
	if(abr == NULL){return n;}
	if(strcmp ( n -> truc -> data.sta.nom , abr -> truc -> data.sta.nom)<0){
		abr -> g = inserer_abr (abr -> g , n);
	}
	else{
		abr -> d =inserer_abr(abr -> d ,n);
	}
	return abr;
}




Un_nabr *construire_abr(Un_elem *liste_sta){
	Un_nabr *abr=NULL;
	while(liste_sta){
		abr=inserer_abr(abr, creer_nabr(liste_sta -> truc));
		liste_sta = liste_sta -> suiv;
	}
	return abr;
}

void detruire_abr(Un_nabr *abr){
	if(abr==NULL){return;}
	detruire_abr(abr -> g);
	detruire_abr(abr -> d);
	free(abr);
}

Un_truc *chercher_station(Un_nabr *abr, char *nom){
	int cmp;
	if(abr==NULL){return NULL;}
	cmp = strcmp (nom , abr -> truc -> data.sta.nom);
	if(cmp==0){return abr -> truc;}
	if(cmp<0){return chercher_station(abr -> g, nom);}
	else{return chercher_station(abr -> d, nom);}
}

