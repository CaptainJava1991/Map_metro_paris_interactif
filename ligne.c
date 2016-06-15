#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "ligne.h"




Une_ligne *lire_lignes(char *nom_fichier){
	Une_ligne *liste_ligne = NULL;
	FILE *f;
	char buffer[300];
	if((f=fopen(nom_fichier,"r"))==NULL){
		fprintf(stderr,"Erreur du fichier dans lire_ligne");
		return NULL;
	}
	char *t;
	
	
	 while(fgets(buffer,300,f)){
		Une_ligne *nl;
		nl = malloc(sizeof(Une_ligne));
		if(nl==NULL){
			fprintf(stderr,"Erreur du malloc de nl dans lire_ligne");
			return NULL;
		}
	 	t = strtok(buffer, ";");
	 	nl -> code = strdup(t);
	 	t = strtok(NULL, ";");
	 	nl -> vitesse = atof(t);
	 	t = strtok(NULL, ";");
	 	nl -> intervalle = atof(t);
	 	t = strtok(NULL, "\n");
	 	nl -> color = strdup(t);
	 	nl -> suiv = liste_ligne;
	 	liste_ligne = nl;
	 }
	 
	 fclose(f);
	 return liste_ligne;
}



void afficher_lignes(Une_ligne *lligne){
	while(lligne){
		printf("%s \t",lligne -> code);
		printf(";");
		printf("%f \t",lligne -> vitesse);
		printf(";");
		printf("%f \t",lligne -> intervalle);
		printf(";");
		printf("%s \n", lligne -> color);
		lligne = lligne -> suiv;
	}
}

void detruire_lignes(Une_ligne *lligne){
	while(lligne){
		Une_ligne *next = lligne -> suiv;
		free(lligne -> code);
		free(lligne -> color);
		free(lligne);
		lligne = next;
	}
}

Une_ligne *chercher_ligne2(Une_ligne *lligne, char *code){
	if(lligne == NULL){
	fprintf(stderr,"Cette ligne n'existe pas dans lligne par chercher_ligne \n");	
	return NULL;
	}
	if(strcmp(code, lligne -> code )==0){
		return lligne;
	}
	return chercher_ligne(lligne -> suiv, code);
}

Une_ligne *chercher_ligne(Une_ligne *lligne, char *code){
	while(lligne){
		if(strcmp(code, lligne -> code)==0){
			return lligne;
		}
	lligne = lligne -> suiv;
	}
	fprintf(stderr,"Cette ligne n'existe pas dans lligne par chercher_ligne \n");
	return NULL;
}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	 	
	 	
