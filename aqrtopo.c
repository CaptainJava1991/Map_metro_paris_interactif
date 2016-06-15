#include "aqrtopo.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "liste.h"
#include "truc.h"

Un_noeud *creer_noeud(Une_coord limite_no,Une_coord limite_se,Un_truc *truc){
  
  Un_noeud *nd;
  nd = malloc(sizeof(Un_noeud));
  if(nd == NULL){
    fprintf(stderr,"Erreur d'allocation de mémoire AQR \n");
    Un_noeud *nd;
    return NULL;
  }
  nd->truc = truc;
  nd->limite_no = limite_no;
  nd->limite_se = limite_se;
  nd->no = NULL;nd->se = NULL;
  nd->so = NULL;nd->ne = NULL;
  return nd;
  
}

	
Un_noeud *inserer_aqr(Un_noeud *aqr, Une_coord limite_no, Une_coord limite_se, Un_truc *truc){
	
		Une_coord pivot,lm_se,lm_no;
		if(!aqr){
			return creer_noeud(limite_no,limite_se,truc);
		}
		if(aqr->truc){
			Un_truc* tf=aqr->truc;
			aqr->truc=NULL;
			if(fabs(tf->coord.lat-truc->coord.lat)<1e-4) tf->coord.lat+=1e-4;
			if(fabs(tf->coord.lon-truc->coord.lon)<1e-4) tf->coord.lon+=1e-4;
			return inserer_aqr(inserer_aqr(aqr,limite_no,limite_se,truc),limite_no,limite_se,tf);
			}

		pivot.lon=(aqr->limite_no.lon+aqr->limite_se.lon)/2;
		pivot.lat=(aqr->limite_no.lat+aqr->limite_se.lat)/2;
		if(truc->coord.lon<pivot.lon){
			lm_no.lon=limite_no.lon;
			lm_se.lon=pivot.lon;
			if(truc->coord.lat<pivot.lat){
				lm_se.lat=limite_se.lat;
				lm_no.lat=pivot.lat;
				aqr->so=inserer_aqr(aqr->so,lm_no,lm_se,truc);
				}
			else{
				lm_se.lat=pivot.lat;
				lm_no.lat=limite_no.lat;
				aqr->no=inserer_aqr(aqr->no,lm_no,lm_se,truc);
				}
			}
		else{
			lm_no.lon=pivot.lon;
			lm_se.lon=limite_se.lon;
			if(truc->coord.lat>pivot.lat){
				lm_se.lat=pivot.lat;
				lm_no.lat=limite_no.lat;
				aqr->ne=inserer_aqr(aqr->ne,lm_no,lm_se,truc);
				}
			else{
				lm_se.lat=limite_se.lat;
				lm_no.lat=pivot.lat;
				aqr->se=inserer_aqr(aqr->se,lm_no,lm_se,truc);

				}		
			}
			
		return aqr;
}
			
Un_noeud* construire_aqr(Un_elem* liste){
	Un_noeud* aqr = NULL;
	Une_coord limite_no, limite_se;
	limites_zone(liste, &limite_no, &limite_se);
	while(liste){
		inserer_aqr(aqr, limite_no, limite_se, liste->truc);
		liste = liste->suiv;
	}
	return aqr;
}


void detruire_aqr(Un_noeud *aqr){
	if(aqr){
		//detruire_truc(aqr->truc);
		detruire_aqr(aqr->no);
		detruire_aqr(aqr->so);
		detruire_aqr(aqr->ne);
		detruire_aqr(aqr->se);
		free(aqr);
	}
}

Un_truc *chercher_aqr(Un_noeud *aqr, Une_coord coord){
	if(aqr == NULL)
	return NULL;
	if(aqr->truc != NULL)
		return aqr->truc;
		Une_coord pivot;
		pivot.lat = (aqr->limite_no.lat+aqr->limite_se.lat)/2;
		pivot.lon = (aqr->limite_no.lon+aqr->limite_se.lon)/2;
	if(coord.lat<pivot.lat){
		if(coord.lon<pivot.lon){
			return chercher_aqr(aqr->so, coord);
		}
		return chercher_aqr(aqr->se, coord);
	}
		if(coord.lon<pivot.lon){
			return chercher_aqr(aqr->no, coord);
		}
	return chercher_aqr(aqr->ne, coord);
}


Un_elem *chercher_zone(Un_noeud *aqr, Un_elem *liste, Une_coord limite_no, Une_coord limite_se){
	if(!aqr) return liste;	
	if (aqr->truc) {
		if ((aqr->truc->coord.lon >= limite_no.lon) && (aqr->truc->coord.lon <= limite_se.lon) && (aqr->truc->coord.lat >= limite_se.lat) && (aqr->truc->coord.lat <= limite_no.lat)) 
			return inserer_liste_trie(liste, aqr->truc);
		else return liste;
	}
	
	liste=chercher_zone(aqr->ne, liste, limite_no, limite_se);
	liste=chercher_zone(aqr->se, liste, limite_no, limite_se);
	liste=chercher_zone(aqr->no, liste, limite_no, limite_se);
	liste=chercher_zone(aqr->so, liste, limite_no, limite_se);
	return liste;
}			
	

				
			
			
			
		
