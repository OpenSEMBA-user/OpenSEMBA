*loop materials *notused
*if(strcmp(Matprop(TypeId),"SIBC")==0)
*if(strcmp(Matprop(SurfaceType),"Layers")==0)
*if(strcmp(Matprop(0),"Surface_Impedance")!=0)
*MatProp(0)
*set var N=Matprop(Layers,int)
 layers     *operation(Matprop(Layers,int)/5)
*for(i=1;i<=N;i=i+5)
*set var j=operation(i/5+1)
*set var numEps=operation(i+1)
*set var relEps=matprop(layers,int,*numEps)
*set var numMu=operation(i+2)
*set var relMu=matprop(layers,int,*numMu)
 eps        *j *operation(relEps*8.85419e-12)
 mu         *j *operation(relMu*1.25664e-6)
 sigma      *j   *matprop(layers,*operation(i+3))
 thickness  *j   *matprop(layers,*i)
*endfor
 fmin           *matprop(Freq_min)
 fmax           *matprop(Freq_max)
 order          *matprop(Number_of_poles)
*endif
*endif
*endif
*end materials
*loop materials 
*if(strcmp(Matprop(TypeId),"SIBC")==0)
*if(strcmp(Matprop(SurfaceType),"Layers")==0)
*MatProp(0)
*set var N=Matprop(Layers,int)
 layers     *operation(Matprop(Layers,int)/5)
*for(i=1;i<=N;i=i+5)
*set var j=operation(i/5+1)
*set var numEps=operation(i+1)
*set var relEps=matprop(layers,int,*numEps)
*set var numMu=operation(i+2)
*set var relMu=matprop(layers,int,*numMu)
 eps        *j *operation(relEps*8.85419e-12)
 mu         *j *operation(relMu*1.25664e-6)
 sigma      *j   *matprop(layers,*operation(i+3))
 thickness  *j   *matprop(layers,*i)
*endfor
 fmin           *matprop(Freq_min)
 fmax           *matprop(Freq_max)
 order          *matprop(Number_of_poles)
*endif
*endif
*end materials
