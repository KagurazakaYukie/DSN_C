#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MArrayList.h"
#include "mtmm.h"
#include "dsn.h"

/*int main(){
	char *h="a:{u=0,gg,hv=d:{j=1,h=2,g=8},g=3,ui=4,ug=cx:{k=1,x=4,hs=8}}";
	char *j="ug";
	char *nw="jdk";
	MTMemoryManager *mm=MTMemoryManagerInit(1024*1024*1024);
	MemoryInfo *mi=MTMemoryManagerCalloc(mm,1024*1024);
	MemoryInfo *jk=MTMemoryManagerCalloc(mm,1024*1024);
	MemoryInfo *nnew=MTMemoryManagerCalloc(mm,1024*1024);
	strcpy(mi->m,h);
	strcpy(jk->m,j);
	strcpy(nnew->m,nw);
	MemoryInfo *ak=DSNGetData(mm,mi,jk);
	printf("@%s\n",ak->m);
	MTMemoryManagerFree(mm,mi);
	MTMemoryManagerFree(mm,jk);
	MTMemoryManagerFree(mm,ak);
	MTMemoryManagerFree(mm,nnew);
	MemoryUnitCheck(mm->mu);
	
	if(DSNCmp(mm,mi,jk,"3")){
		printf("true");
	}else{
		printf("false");
	}
	
	DSN *d=DSNInit(mm);
	Analysis(mm,d,mi);
	for(int i=0;i<MArrayListSize(d->arraylist);i++){
		printf("%s\n",((DSN*)MArrayListGetIndex(d->arraylist,i))->d->m);
		MemoryInfo *hj=((DSN*)MArrayListGetIndex(d->arraylist,i))->dn;
		if(hj!=NULL){
			printf("%s\n",hj->m);
		}
	}
	
	
}*/

DSN *DSNInit(MTMemoryManager *mm){
	MemoryInfo *m=MTMemoryManagerCalloc(mm,sizeof(DSN));
	DSN *dsn=m->m;
	dsn->m=m;
	dsn->arraylist=MArrayListInit(mm,1024*1024);
	return dsn;
}

void DSNDestroy(MTMemoryManager *mm,DSN *dsn){
	DSN *d;
	for(int i=0;i<MArrayListSize(dsn->arraylist);i++){
		d=(DSN*)MArrayListGetIndex(dsn->arraylist,i);
		MTMemoryManagerFree(mm,d->d);
		if(d->dn!=NULL){
			MTMemoryManagerFree(mm,d->dn);
		}
		MTMemoryManagerFree(mm,d->m);
	}
	MTMemoryManagerFree(mm,dsn->n);
	MArrayListDestroy(mm,dsn->arraylist);
	MTMemoryManagerFree(mm,dsn->m);
}

MemoryInfo *CreateDSNData(MTMemoryManager *mm,char *name,char **dataname,char **data,int size){
	MemoryInfo *mi=MTMemoryManagerCalloc(mm,1024*1024);
	if(name!=NULL|!strcmp(name,"")){
		strcat(mi->m,name);
		strcat(mi->m,":");
	}
	strcat(mi->m,"{");
	int b=size-1;
	for(int i=0;i<size;i++){
		if(!strcmp(dataname[i],"")|dataname[i]!=NULL){
			strcat(mi->m,dataname[i]);
			strcat(mi->m,"=");
			strcat(mi->m,data[i]);
		}else{
			strcat(mi->m,data[i]);
		}
		if(i!=b){
				strcat(mi->m,",");
		}
	}
	strcat(mi->m,"}");
	return mi;
}

bool CharIntervalCmp(char *a,int aq,int aw,char *b){
	int i=0;
	if(aw-aq==strlen(b)){
		for(;aq<aw;aq++){
			if(a[aq]!=b[i]){
				return false;
			}
			i++;
		}
	}else{
		return false;
	}
	return true;
}

MemoryInfo *DSNGetData(MTMemoryManager *mm,MemoryInfo *d,MemoryInfo *Name){
	int name=0,cs=0,dh=0,hk=0,dy=0,init=IndexOf(0,d->m,'{')+1;
	MemoryInfo *sd=SubString(mm,d->m,init,strlen(d->m)-1),*si=NULL,*tmp=NULL;
	while(true){
		dh=IndexOf(dh,sd->m,',');
		if(dh!=-1){
			if(si!=NULL){
				MTMemoryManagerFree(mm,si);
			}
			si=SubString(mm,sd->m,cs,dh);
			hk=IndexOf(0,si->m,'{');
			if(hk!=-1){
				if(CharIntervalCmp(si->m,0,IndexOf(0,si->m,'='),Name->m)){
					name+=init+cs;
					MTMemoryManagerFree(mm,si);
					MTMemoryManagerFree(mm,sd);
					return SubString(mm,d->m,name+strlen(Name->m)+1,IndexOf(name,d->m,'}')+1);
				}
				cs=IndexOf(cs,sd->m,'}')+2;
				dh=IndexOf(cs,sd->m,',');
				continue;
			}else{
				if(tmp!=NULL){
					MTMemoryManagerFree(mm,tmp);
				}
				tmp=SubString(mm,sd->m,cs,dh);
				if((dy=IndexOf(0,tmp->m,'='))!=-1){
					if(CharIntervalCmp(tmp->m,0,dy,Name->m)){
						name+=init+cs;
						MTMemoryManagerFree(mm,tmp);
						MTMemoryManagerFree(mm,si);
						MTMemoryManagerFree(mm,sd);
						return SubString(mm,d->m,name+strlen(Name->m)+1,IndexOf(name,d->m,','));
					}
				}else{
					if(strcmp(tmp->m,Name->m)==0){
						name+=init+cs;
						MTMemoryManagerFree(mm,tmp);
						MTMemoryManagerFree(mm,si);
						MTMemoryManagerFree(mm,sd);
						return SubString(mm,d->m,name,IndexOf(name,d->m,','));
					}
				}
			}
		}else{
			dh=strlen(sd->m);
			if(cs>dh){
				break;
			}
			if(tmp!=NULL){
				MTMemoryManagerFree(mm,tmp);
			}
			tmp=SubString(mm,sd->m,cs,dh);
			if((dy=IndexOf(0,tmp->m,'='))!=-1){
					if(CharIntervalCmp(tmp->m,0,dy,Name->m)){
						name+=init+cs;
						MTMemoryManagerFree(mm,tmp);
						MTMemoryManagerFree(mm,si);
						MTMemoryManagerFree(mm,sd);
						return SubString(mm,d->m,name+strlen(Name->m)+1,strlen(d->m)-1);
					}
				}else{
					if(strcmp(tmp->m,Name->m)==0){
						name+=init+cs;
						MTMemoryManagerFree(mm,tmp);
						MTMemoryManagerFree(mm,si);
						MTMemoryManagerFree(mm,sd);
						return SubString(mm,d->m,name,strlen(d->m)-1);
					}
				}
			break;
		}
		dh++;
		cs=dh;
	}
	MTMemoryManagerFree(mm,tmp);
	MTMemoryManagerFree(mm,si);
	MTMemoryManagerFree(mm,sd);
	return NULL;
}



bool DSNNameCmp(void *a,void *b){
	return CharIntervalCmp(a,0,IndexOf(0,a,':'),b);
}

MemoryInfo *DSNSetData(MTMemoryManager *mm,MemoryInfo *d,MemoryInfo *Name,MemoryInfo *nd){
	int name=0,cs=0,dh=0,hk=0,dy=0,init=IndexOf(0,d->m,'{')+1;
	MemoryInfo *sd=SubString(mm,d->m,init,strlen(d->m)-1),*si=NULL,*tmp=NULL;
	while(true){
		dh=IndexOf(dh,sd->m,',');
		if(dh!=-1){
			if(si!=NULL){
				MTMemoryManagerFree(mm,si);
			}
			si=SubString(mm,sd->m,cs,dh);
			hk=IndexOf(0,si->m,'{');
			if(hk!=-1){
				if(CharIntervalCmp(si->m,0,IndexOf(0,si->m,'='),Name->m)){
					name+=init+cs;
					MTMemoryManagerFree(mm,si);
					MTMemoryManagerFree(mm,sd);
					MemoryInfo *ndata=MTMemoryManagerCalloc(mm,1024*1024);
					MemoryInfo *n1=SubString(mm,d->m,0,name+strlen(Name->m)+1);
					MemoryInfo *n2=SubString(mm,d->m,IndexOf(name,d->m,'}')+1,strlen(d->m));
					strcat(ndata->m,n1->m);
					strcat(ndata->m,nd->m);
					strcat(ndata->m,n2->m);
					MTMemoryManagerFree(mm,d);
					MTMemoryManagerFree(mm,n1);
					MTMemoryManagerFree(mm,n2);
					return ndata;
				}
				cs=IndexOf(cs,sd->m,'}')+2;
				dh=IndexOf(cs,sd->m,',');
				continue;
			}else{
				if(tmp!=NULL){
					MTMemoryManagerFree(mm,tmp);
				}
				tmp=SubString(mm,sd->m,cs,dh);
				if((dy=IndexOf(0,tmp->m,'='))!=-1){
					if(CharIntervalCmp(tmp->m,0,dy,Name->m)){
						name+=init+cs;
						MTMemoryManagerFree(mm,tmp);
						MTMemoryManagerFree(mm,si);
						MTMemoryManagerFree(mm,sd);
						MemoryInfo *ndata=MTMemoryManagerCalloc(mm,1024*1024);
						MemoryInfo *n1=SubString(mm,d->m,0,name+strlen(Name->m)+1);
						MemoryInfo *n2=SubString(mm,d->m,IndexOf(name,d->m,','),strlen(d->m));
						strcat(ndata->m,n1->m);
						strcat(ndata->m,nd->m);
						strcat(ndata->m,n2->m);
						MTMemoryManagerFree(mm,d);
						MTMemoryManagerFree(mm,n1);
						MTMemoryManagerFree(mm,n2);
						return ndata;
					}
				}else{
					if(strcmp(tmp->m,Name->m)==0){
						name+=init+cs;
						MTMemoryManagerFree(mm,tmp);
						MTMemoryManagerFree(mm,si);
						MTMemoryManagerFree(mm,sd);
						MemoryInfo *ndata=MTMemoryManagerCalloc(mm,1024*1024);
						MemoryInfo *n1=SubString(mm,d->m,0,name);
						MemoryInfo *n2=SubString(mm,d->m,name+strlen(Name->m),strlen(d->m));
						strcat(ndata->m,n1->m);
						strcat(ndata->m,nd->m);
						strcat(ndata->m,n2->m);
						MTMemoryManagerFree(mm,d);
						MTMemoryManagerFree(mm,n1);
						MTMemoryManagerFree(mm,n2);
						return ndata;
					}
				}
			}
		}else{
			dh=strlen(sd->m);
			if(cs>dh){
				break;
			}
			if(tmp!=NULL){
				MTMemoryManagerFree(mm,tmp);
			}
			tmp=SubString(mm,sd->m,cs,dh);
			if((dy=IndexOf(0,tmp->m,'='))!=-1){
					if(CharIntervalCmp(tmp->m,0,dy,Name->m)){
						name+=init+cs;
						MTMemoryManagerFree(mm,tmp);
						MTMemoryManagerFree(mm,si);
						MTMemoryManagerFree(mm,sd);
						MemoryInfo *ndata=MTMemoryManagerCalloc(mm,1024*1024);
						MemoryInfo *n1=SubString(mm,d->m,0,name+strlen(Name->m)+1);
						MemoryInfo *n2=SubString(mm,d->m,name+strlen(Name->m),strlen(d->m));
						strcat(ndata->m,n1->m);
						strcat(ndata->m,nd->m);
						strcat(ndata->m,n2->m);
						MTMemoryManagerFree(mm,d);
						MTMemoryManagerFree(mm,n1);
						MTMemoryManagerFree(mm,n2);
						return ndata;
					}
				}else{
					if(strcmp(tmp->m,Name->m)==0){
						name+=init+cs;
						MTMemoryManagerFree(mm,tmp);
						MTMemoryManagerFree(mm,si);
						MTMemoryManagerFree(mm,sd);
						MemoryInfo *ndata=MTMemoryManagerCalloc(mm,1024*1024);
						MemoryInfo *n1=SubString(mm,d->m,0,name);
						MemoryInfo *n2=SubString(mm,d->m,name+strlen(Name->m),strlen(d->m));
						strcat(ndata->m,n1->m);
						strcat(ndata->m,nd->m);
						strcat(ndata->m,n2->m);
						MTMemoryManagerFree(mm,d);
						MTMemoryManagerFree(mm,n1);
						MTMemoryManagerFree(mm,n2);
						return ndata;
					}
				}
			break;
		}
		dh++;
		cs=dh;
	}
	MTMemoryManagerFree(mm,tmp);
	MTMemoryManagerFree(mm,si);
	MTMemoryManagerFree(mm,sd);
	return NULL;
}

MemoryInfo *DSNDeleteData(MTMemoryManager *mm,MemoryInfo *d,MemoryInfo *Name){
	int name=0,cs=0,dh=0,hk=0,dy=0,init=IndexOf(0,d->m,'{')+1;
	MemoryInfo *sd=SubString(mm,d->m,init,strlen(d->m)-1),*si=NULL,*tmp=NULL;
	while(true){
		dh=IndexOf(dh,sd->m,',');
		if(dh!=-1){
			if(si!=NULL){
				MTMemoryManagerFree(mm,si);
			}
			si=SubString(mm,sd->m,cs,dh);
			hk=IndexOf(0,si->m,'{');
			if(hk!=-1){
				if(CharIntervalCmp(si->m,0,IndexOf(0,si->m,'='),Name->m)){
					name+=init+cs;
					MTMemoryManagerFree(mm,si);
					MTMemoryManagerFree(mm,sd);
					MemoryInfo *ndata=MTMemoryManagerCalloc(mm,1024*1024);
					MemoryInfo *n1=SubString(mm,d->m,0,name-1);
					MemoryInfo *n2=SubString(mm,d->m,IndexOf(name,d->m,'}')+1,strlen(d->m));
					strcat(ndata->m,n1->m);
					strcat(ndata->m,n2->m);
					MTMemoryManagerFree(mm,d);
					MTMemoryManagerFree(mm,n1);
					MTMemoryManagerFree(mm,n2);
					return ndata;
				}
				cs=IndexOf(cs,sd->m,'}')+2;
				dh=IndexOf(cs,sd->m,',');
				continue;
			}else{
				if(tmp!=NULL){
					MTMemoryManagerFree(mm,tmp);
				}
				tmp=SubString(mm,sd->m,cs,dh);
				if((dy=IndexOf(0,tmp->m,'='))!=-1){
					if(CharIntervalCmp(tmp->m,0,dy,Name->m)){
						name+=init+cs;
						MTMemoryManagerFree(mm,tmp);
						MTMemoryManagerFree(mm,si);
						MTMemoryManagerFree(mm,sd);
						MemoryInfo *ndata=MTMemoryManagerCalloc(mm,1024*1024);
						MemoryInfo *n1=SubString(mm,d->m,0,name-1);
						MemoryInfo *n2=SubString(mm,d->m,IndexOf(name,d->m,','),strlen(d->m));
						strcat(ndata->m,n1->m);
						strcat(ndata->m,n2->m);
						MTMemoryManagerFree(mm,d);
						MTMemoryManagerFree(mm,n1);
						MTMemoryManagerFree(mm,n2);
						return ndata;
					}
				}else{
					if(strcmp(tmp->m,Name->m)==0){
						name+=init+cs;
						MTMemoryManagerFree(mm,tmp);
						MTMemoryManagerFree(mm,si);
						MTMemoryManagerFree(mm,sd);
						MemoryInfo *ndata=MTMemoryManagerCalloc(mm,1024*1024);
						MemoryInfo *n1=SubString(mm,d->m,0,name-1);
						MemoryInfo *n2=SubString(mm,d->m,name+strlen(Name->m),strlen(d->m));
						strcat(ndata->m,n1->m);
						strcat(ndata->m,n2->m);
						MTMemoryManagerFree(mm,d);
						MTMemoryManagerFree(mm,n1);
						MTMemoryManagerFree(mm,n2);
						return ndata;
					}
				}
			}
		}else{
			dh=strlen(sd->m);
			if(cs>dh){
				break;
			}
			if(tmp!=NULL){
				MTMemoryManagerFree(mm,tmp);
			}
			tmp=SubString(mm,sd->m,cs,dh);
			if((dy=IndexOf(0,tmp->m,'='))!=-1){
					if(CharIntervalCmp(tmp->m,0,dy,Name->m)){
						name+=init+cs;
						MTMemoryManagerFree(mm,tmp);
						MTMemoryManagerFree(mm,si);
						MTMemoryManagerFree(mm,sd);
						MemoryInfo *ndata=MTMemoryManagerCalloc(mm,1024*1024);
						MemoryInfo *n1=SubString(mm,d->m,0,name-1);
						MemoryInfo *n2=SubString(mm,d->m,name+strlen(tmp->m),strlen(d->m));
						strcat(ndata->m,n1->m);
						strcat(ndata->m,n2->m);
						MTMemoryManagerFree(mm,d);
						MTMemoryManagerFree(mm,n1);
						MTMemoryManagerFree(mm,n2);
						return ndata;
					}
				}else{
					if(strcmp(tmp->m,Name->m)==0){
						name+=init+cs;
						MTMemoryManagerFree(mm,tmp);
						MTMemoryManagerFree(mm,si);
						MTMemoryManagerFree(mm,sd);
						MemoryInfo *ndata=MTMemoryManagerCalloc(mm,1024*1024);
						MemoryInfo *n1=SubString(mm,d->m,0,name-1);
						MemoryInfo *n2=SubString(mm,d->m,name+strlen(tmp->m),strlen(d->m));
						strcat(ndata->m,n1->m);
						strcat(ndata->m,n2->m);
						MTMemoryManagerFree(mm,d);
						MTMemoryManagerFree(mm,n1);
						MTMemoryManagerFree(mm,n2);
						return ndata;
					}
				}
			break;
		}
		dh++;
		cs=dh;
	}
	MTMemoryManagerFree(mm,tmp);
	MTMemoryManagerFree(mm,si);
	MTMemoryManagerFree(mm,sd);
	return NULL;
}

bool DSNCmp(MTMemoryManager *mm,MemoryInfo *d,MemoryInfo *name,char *q){
	MemoryInfo *am=MTMemoryManagerCalloc(mm,1024*1024);
	MemoryInfo *gm=MTMemoryManagerCalloc(mm,1024*1024);
	MemoryInfo *tmp=MTMemoryManagerCalloc(mm,1024*1024);
	
	char *a=(char*)am->m;
	char *g=(char*)gm->m;
	char *dm=(char*)d->m;
	
	char *dmh=strchr(dm,':');
	char *dhk=strchr(dm,'{');
	
	if(dmh!=NULL){
		strncpy(tmp->m,dm,1);
		if(strcmp((char*)tmp->m,"{")!=0){
			strncpy(g,dmh+2,strlen(dm)-4);
		}else{
	        strncpy(g,dhk+1,strlen(dm)-2);
		}
	}else{
		strncpy(g,dhk+1,strlen(dm)-2);
	}
	bool re=false;
	int y=0;
	MemoryInfo *as;
	while(true){
		if(IndexOf(y,g,',')!=-1){
			as=SubString(mm,g,y,IndexOf(y,g,','));
			if(IndexOf(0,as->m,'=')!=-1){
				if(IndexOf(0,as->m,'{')!=-1){
					y=IndexOf(y,g,'}')+2;
				}else{
					if(CharIntervalCmp(g,y,IndexOf(y,g,'='),name->m)){
						if(CharIntervalCmp(g,IndexOf(y,g,'=')+1,IndexOf(y,g,','),q)){
							MTMemoryManagerFree(mm,as);
							re=true;
							break;
						}
					}
					y=IndexOf(y,g,',')+1;
				}
			}else{
				if(strcmp(as->m,q)==0){
					MTMemoryManagerFree(mm,as);
					re=true;
					break;
				}
				y=IndexOf(y,g,',')+1;
			}
			MTMemoryManagerFree(mm,as);
		}else{
			if(y>strlen(g)){
				break;
			}
			as=SubString(mm,g,y,strlen(g));
			if(IndexOf(0,as->m,'=')!=-1){
				if(IndexOf(0,as->m,'{')!=-1){
					break;
				}else{
					if(CharIntervalCmp(g,y,IndexOf(y,g,'='),name->m)){
						if(CharIntervalCmp(g,IndexOf(y,g,'=')+1,strlen(g),q)){
							MTMemoryManagerFree(mm,as);
							re=true;
							break;
						}
					}
					MTMemoryManagerFree(mm,as);
					break;
				}
			}else{
				if(strcmp(as->m,q)==0){
					re=true;
				}
				MTMemoryManagerFree(mm,as);
				break;
			}
		}
	}
	MTMemoryManagerFree(mm,tmp);
	MTMemoryManagerFree(mm,gm);
	MTMemoryManagerFree(mm,am);
	return re;
}

bool IsDSN(char *m){
	return IndexOf(0,m,'{')&&IndexOf(0,m,'}');
}

MemoryInfo *GetDSNName(MTMemoryManager *mm,MemoryInfo *d){
	MemoryInfo *in=MTMemoryManagerCalloc(mm,1024*1024);
	strncpy(in->m,d->m,strchr(((char*)(d->m)),':')-((char*)(d->m)));
	return in;
}

int IndexOf(int i,char *a,char b){
	for(i;i<strlen(a);i++){
		if(a[i]==b){
			return i;
		}
	}
	return -1;
}

MemoryInfo *SubString(MTMemoryManager *mm,char *a,int b,int c){
	if(b>=0&&c>=0){
		MemoryInfo *m=MTMemoryManagerCalloc(mm,strlen(a));
		strncpy(m->m,a+b,c-b);
		return m;
	}else{
		return NULL;
	}
}

DSN *NewDSN(MTMemoryManager *mm,MemoryInfo *name,MemoryInfo *dn,MemoryInfo *d){
	MemoryInfo *dsnm=MTMemoryManagerCalloc(mm,sizeof(DSN));
	DSN *dsn=(DSN*)dsnm->m;
	dsn->n=name;
	dsn->m=dsnm;
	dsn->d=d;
	dsn->dn=dn;
	return dsn;
}

DSN *Analysis(MTMemoryManager *mm,DSN *dsn,MemoryInfo *d){
	MemoryInfo *n=MTMemoryManagerCalloc(mm,1024*1024);
	MemoryInfo *am=MTMemoryManagerCalloc(mm,1024*1024);
	MemoryInfo *gm=MTMemoryManagerCalloc(mm,1024*1024);
	MemoryInfo *tmp=MTMemoryManagerCalloc(mm,1024*1024);
	
	char *a=(char*)am->m;
	char *g=(char*)gm->m;
	char *dm=(char*)d->m;
	
	char *dmh=strchr(dm,':');
	char *dhk=strchr(dm,'{');
	
	if(dmh!=NULL){
		strncpy(tmp->m,dm,1);
		if(strcoll((char*)tmp->m,"{")!=0){
			strncpy(n->m,dm,dmh-dm);
			dsn->n=n;
			strncpy(g,dmh+2,strlen(dm)-4);
		}else{
	        strncpy(g,dhk+1,strlen(dm)-2);
		}
	}else{
		strncpy(g,dhk+1,strlen(dm)-2);
	}
	
	int y=0;
	MemoryInfo *as;
	while(true){
		if(IndexOf(y,g,',')!=-1){
			as=SubString(mm,g,y,IndexOf(y,g,','));
			if(IndexOf(0,as->m,'=')!=-1){
				if(IndexOf(0,as->m,'{')!=-1){
					MemoryInfo *sz=SubString(mm,g,IndexOf(y,g,'=')+1,IndexOf(y,g,'}')+1);
					MemoryInfo *mnz=SubString(mm,g,y,IndexOf(y,g,'='));
					MArrayListAddIndex(mm,dsn->arraylist,NewDSN(mm,n,mnz,sz));
					y=IndexOf(y,g,'}')+2;
				}else{
					MemoryInfo *mz=SubString(mm,g,y,IndexOf(y,g,'='));
					MemoryInfo *d=SubString(mm,g,IndexOf(y,g,'=')+1,IndexOf(y,g,','));
					MArrayListAddIndex(mm,dsn->arraylist,NewDSN(mm,n,mz,d));
					y=IndexOf(y,g,',')+1;
				}
			}else{
				MemoryInfo *yg=MTMemoryManagerCalloc(mm,as->size);
				strcpy(yg->m,as->m);
				MArrayListAddIndex(mm,dsn->arraylist,NewDSN(mm,n,NULL,yg));
				y=IndexOf(y,g,',')+1;
			}
			MTMemoryManagerFree(mm,as);
		}else{
			if(y>strlen(g)){
				break;
			}
			as=SubString(mm,g,y,strlen(g));
			if(IndexOf(0,as->m,'=')!=-1){
				if(IndexOf(0,as->m,'{')!=-1){
					MemoryInfo *sz=SubString(mm,g,IndexOf(y,g,'=')+1,strlen(g));
					MemoryInfo *mnz=SubString(mm,g,y,IndexOf(y,g,'='));
					MTMemoryManagerFree(mm,as);
					MArrayListAddIndex(mm,dsn->arraylist,NewDSN(mm,n,mnz,sz));
				}else{
					MemoryInfo *mz=SubString(mm,g,y,IndexOf(y,g,'='));
					MemoryInfo *d=SubString(mm,g,IndexOf(y,g,'=')+1,strlen(g));
					MTMemoryManagerFree(mm,as);
					MArrayListAddIndex(mm,dsn->arraylist,NewDSN(mm,n,mz,d));
				}
			}else{
				MemoryInfo *yg=MTMemoryManagerCalloc(mm,as->size);
				strcpy(yg->m,as->m);
				MTMemoryManagerFree(mm,as);
				MArrayListAddIndex(mm,dsn->arraylist,NewDSN(mm,n,NULL,yg));
			}
			break;
		}
	}
	MTMemoryManagerFree(mm,tmp);
	MTMemoryManagerFree(mm,gm);
	MTMemoryManagerFree(mm,am);
}
