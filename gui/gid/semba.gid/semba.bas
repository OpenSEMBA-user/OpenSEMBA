{
    "_format": "Semba Data File in JSON format ",
    "_version": "*tcl(set version $semba::VersionNumber)",

*# ----------------------------------------------------------
*# ------------------ SOLVER OPTIONS ------------------------
*# ----------------------------------------------------------
    "solverOptions": {
        "solverName": "*GenData(Solver)",
        "runSimulation": *GenData(Run_simulation),
*if(strcasecmp(GenData(Ending),"Final_time")==0)
        "finalTime": *GenData(Final_time),
*else
        "numberOfTimeSteps": *GenData(Number_of_time_steps),
*endif
        "timeStep": *GenData(Time_step),
        "cfl": *GenData(CFL),
        "defaultSamplingPeriod": *GenData(Default_sampling_period),
        "forceRestarting": *GenData(Force_restarting),
        "resumeSimulation": *GenData(Resume_simulation),
        "flush": *GenData(Flush),
*if(strcmp(GenData(Additional_arguments),"")!=0)
        "additionalArguments": *GenData(Additional_arguments),
*endif
*# ----
*if(strcasecmp(GenData(Solver),"ugrfdtd")==0)
        "compositesModel": "*GenData(Composites_model)",
        "wiresFlavor": "*GenData(Wires_flavor)",
        "mtln": *GenData(MTLN),
        "minDistanceWires": *GenData(Min_distance_wires),
        "mapVTK": *GenData(Map_VTK),
        "geometryScalingFactor": *GenData(scaling_factor),
        "mesherOptions": {
            "lowerXBound": "*GenData(Lower_x_bound)",
            "lowerYBound": "*GenData(Lower_y_bound)",
            "lowerZBound": "*GenData(Lower_z_bound)",
            "upperXBound": "*GenData(Upper_x_bound)",
            "upperYBound": "*GenData(Upper_y_bound)",          
            "upperZBound": "*GenData(Upper_z_bound)",
            "mesher": "*GenData(Mesher)",
            "bruteForceVolumes": *GenData(Brute_force_volumes),
            "vtkExport": *GenData(VTK_Export),
            "postmshExport": *GenData(postmsh_Export),
            "mode": "*GenData(Mode)",
            "forbiddenLength": *GenData(Forbidden_length),
            "slantedWires": *GenData(Slanted),
            "slantedThreshold": *GenData(Segments_filter_threshold)
        }
*elseif(strcasecmp(GenData(Solver),"cudg3d")==0)
        "timeIntegrator": *GenData(Time_integrator),
        "useLTS": *GenData(Use_LTS),
        "upwinding": *GenData(Upwinding)
*else
*WarningBox Unrecognized solver
        "_error": "invalidSolver"
*endif
    },
     
*# ----------------------------------------------------------
*# ------------------ PROBLEM SIZE --------------------------
*# ----------------------------------------------------------
    "problemSize": {
*set elems(Hexahedra)
        "hexahedra8":     *nelem,
*set elems(Tetrahedra)
        "tetrahedra4":    *nelem,
*set elems(Quadrilateral)
        "quadrilateral4": *nelem,
*set elems(Triangle)
        "triangle3":      *nelem,
*set elems(Linear)
        "linear2":        *nelem,
*set elems(all)
        "coordinates":    *npoin,
        "materials":      *nmats
    },

*# ----------------------------------------------------------
*# -------------------- MATERIALS ---------------------------
*# ----------------------------------------------------------
    "materials": [
*loop materials
        {
            "materialId": *matnum(),
            "name": "*MatProp(0)",
*if(strcmp(MatProp(TypeId),"PEC")==0)
            "materialType": "*MatProp(TypeId)"
*elseif(strcmp(MatProp(TypeId),"PMC")==0)
            "materialType": "*MatProp(TypeId)"
*elseif(strcmp(MatProp(TypeId),"PML")==0)
            "materialType": "*MatProp(TypeId)",
            "automaticOrientation": *matprop(Automatic_orientation),
*if(strcmp(MatProp(Local_Axes),"-GLOBAL-")==0)
            "localAxes": "{0.0 0.0 0.0} {0.0 0.0 0.0}",
*else
            "localAxes: "*tcl(GiD_Info localaxes *matprop(Local_Axes))",
*endif
*elseif(strcmp(Matprop(TypeId),"Classic")==0)
            "materialType": "*MatProp(TypeId)",
            "permittivity": *matprop(Permittivity),
            "permeability": *matprop(Permeability),
            "electricConductivity": *matprop(ElecCond),
            "magneticConductivity": *matprop(MagnCond)
*elseif(strcmp(Matprop(TypeId),"Dispersive")==0)
            "materialType": "*MatProp(TypeId)",
            "filename": "*matprop(File)"
*elseif(strcmp(Matprop(TypeId),"Wire")==0)
            "materialType": "*MatProp(TypeId)",
            "radius": *matprop(Radius),
            "wireType": "*matprop(WireType)",
*if(strcmp(Matprop(WireType),"Dispersive")==0)
            "filename": *matprop(File)
*elseif(strcmp(Matprop(WireType),"Standard")==0)
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance)
*elseif(strcmp(Matprop(WireType),"SeriesParallel")==0)
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance),
            "capacitance": *matprop(capacitance),   
            "parallelResistance": *matprop(Parallel_Resistance),
            "parallelInductance": *matprop(Parallel_Inductance),
            "parallelCapacitance": *matprop(Parallel_Capacitance)
*else
            "_error": "Invalid wireType"
*endif
*elseif(strcmp(Matprop(TypeId),"Conn_sRLC")==0)
            "materialType": "Connector",
            "connectorType": "*MatProp(TypeId)",
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance),
            "capacitance": *matprop(capacitance)
*elseif(strcmp(Matprop(TypeId),"Conn_pRLC")==0)
            "materialType": "Connector",
            "connectorType": "*MatProp(TypeId)",
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance),
            "capacitance": *matprop(capacitance)
*elseif(strcmp(Matprop(TypeId),"Conn_sLpRC")==0)
            "materialType": "Connector",
            "connectorType": "*MatProp(TypeId)",
            "materialType": "*MatProp(TypeId)",
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance),
            "capacitance": *matprop(capacitance)
*elseif(strcmp(Matprop(TypeId),"Thin_gap")==0)
            "materialType": "*MatProp(TypeId)",
            "width": *matprop(Width)
*elseif(strcmp(Matprop(TypeId),"Conn_dispersive")==0)
            "materialType": "*MatProp(TypeId)",
            "filename": "*matprop(File)"
*elseif(strcmp(Matprop(TypeId),"SIBC")==0)
            "materialType": "*MatProp(TypeId)",
            "surfaceType": "*matprop(SurfaceType)"
*if(strcmp(Matprop(SurfaceType),"File")==0)
            "materialType": "*MatProp(TypeId)",   
            "filename": "*matprop(File)"
*endif
*if(matprop(Layers,int)==1)
*warningbox "Multilayer did not contain layers"
            "_error": "Multilayer did not contain layers",
*else
*set var num_values(int)=matprop(Layers,int)
            "layers": [
*for(i=1;i<=num_values(int);i=i+5)
                {
                    "thickness": *matprop(Layers,*i),
                    "permittivity": *matprop(Layers,*operation(i+1)),
                    "permeability": *matprop(Layers,*operation(i+2)),
                    "elecCond": *matprop(Layers,*operation(i+3)),
                    "magnCond": *matprop(Layers,*operation(i+4))
*if(operation(i+4)!=num_values(int))
                },
*else
                }
*endif
*end for
            ],
*endif
            "freqMin": *matProp(freq_min),
            "freqMax": *matProp(freq_max),
            "numberOfPoles": *matProp(number_Of_Poles)
*elseif(strcmp(Matprop(TypeId),"Anisotropic")==0)
            "materialType": "*MatProp(TypeId)"
*if(strcmp(MatProp(Local_Axes),"-GLOBAL-")==0)
            "localAxes": "{0.0 0.0 0.0} {0.0 0.0 0.0}",
*else
            "localAxes": "*tcl([lindex [GiD_Info localaxes *matprop(Local_Axes)] 9])",
*endif
            "anisotropicModel": "*matprop(Anisotropic_model)",
*if(strcmp(MatProp(anisotropicModel),"Crystal")==0)
            "relativePermittivityPrincipalAxes": "*matprop(Relative_permittivity_principal_axes)",
            "crystalRelativePermeability": *matprop(Crystal_relative_permeability)
*elseif(strcmp(MatProp(anisotropicModel),"Ferrite")==0)
            "kappa": *matprop(Kappa),
            "ferriteRelativePermeability": *matprop(Ferrite_relative_permeability),
            "ferriteRelativePermittivity": *matprop(Ferrite_relative_permittivity)
*else
            "_error": "Unrecognized anisotropic model"
*endif
*else
*warningBox "Unrecognized material label"
            "_error": "Unrecognized material label"
*endif
*if(nmats != loopvar)
        },
*else
        }
*endif
*end materials
    ],

*# ----------------------------------------------------------
*# -------------------- GRIDS -------------------------------
*# ----------------------------------------------------------
    "grids": [
*set var NGRIDS = 0
*if(strcasecmp(GenData(Mesher),"None")!=0)
*set elems(all)
*set Cond Grid
*if(CondNumEntities(int)>0)
*loop layers *OnlyInCond
        {
*set var NGRIDS = NGRIDS + 1
            "gridType": "gridCondition",
            "layerBox": "*tcl(GiD_Info layer -bbox -use geometry *layerName)",
            "type": "*cond(Type)",
*if(strcasecmp(cond(boundary_padding_type),"None")==0)
            "directions": "{*cond(Size)}"       
*else
            "directions": "{*cond(Size)}",
            "boundaryPaddingType":  "*cond(boundary_padding_type)",
            "upperPadding":         "{*cond(Upper_padding)}",
            "lowerPadding":         "{*cond(Lower_padding)}",
            "upperPaddingMeshSize": "{*cond(Upper_padding_mesh_size)}",
            "lowerPaddingMeshSize": "{*cond(Lower_padding_mesh_size)}"
*endif
*if(CondNumEntities(int)!=loopVar)
        },
*else
        }
*endif
*end layers
*endif
*elseif(tcl(expr [GiD_Cartesian get dimension] != -1))
*set var NGRIDS = NGRIDS + 1
        {
            "gridType":    "nativeGiD",
*#            "corner":      "{*tcl(GiD_Cartesian get corner)}",
*#            "boxSize":     "{*tcl(GiD_Cartesian get boxsize)}",
*#            "nGridPoints": "{*tcl(GiD_Cartesian get ngridpoints)}",
            "xCoordinates": *tcl(semba::getGridCoordinatesAsJSONArrayBAS 0),
            "yCoordinates": *tcl(semba::getGridCoordinatesAsJSONArrayBAS 1),
            "zCoordinates": *tcl(semba::getGridCoordinatesAsJSONArrayBAS 2)
        }
*else 
*if(strcasecmp(GenData(Solver),"ugrfdtd")==0)
*WarningBox "No grid defined. Define grid condition or use GiD native mesher."
*if(NGRIDS!=1)
*WarningBox "Several grids defined. Only one grid is allowed for UGRFDTD"
*endif
*endif
*endif
    ],

*# ----------------------------------------------------------
*# -------------------- LAYERS ------------------------------
*# ----------------------------------------------------------
    "layers": [
*set elems(all)
*loop layers
*set var NUMBER_OF_LAYERS = loopVar
*end layers
*loop layers
*if(NUMBER_OF_LAYERS != loopVar)
        {"id": *LayerNum, "name": "*LayerName"},
*else
        {"id": *LayerNum, "name": "*LayerName"}
*endif
*end layers
    ],

*# ----------------------------------------------------------
*# ----------------- COORDINATES ----------------------------
*# ----------------------------------------------------------
    "coordinates": [
*set elems(all)
*loop nodes
*format "%7i %+14.8e %+14.8e %+14.8e"
*if(npoin != loopVar)
        "*NodesNum *NodesCoord(1,real) *NodesCoord(2,real) *NodesCoord(3,real)",
*else
        "*NodesNum *NodesCoord(1,real) *NodesCoord(2,real) *NodesCoord(3,real)"
*endif
*end nodes
    ],

*# ----------------------------------------------------------
*# ------------------- ELEMENTS -----------------------------
*# ----------------------------------------------------------
    "elements": {
*set elems(Hexahedra)
        "hexahedra": [
*loop elems
*format "%8i %7i %7i %7i %7i %7i %7i %7i %7i %3i %3i"
*if(nelem != loopvar)
        "*ElemsNum *ElemsConec *ElemsMat *ElemsLayerNum",
*else
        "*ElemsNum *ElemsConec *ElemsMat *ElemsLayerNum"
*endif        
*end elems
        ],
*set elems(Tetrahedra)
        "tetrahedra": [
*loop elems
*format "%8i %7i %7i %7i %7i %3i %3i"
*if(nelem != loopvar)
        "*ElemsNum *ElemsConec(1) *elemsconec(3) *elemsconec(2) *elemsconec(4) *ElemsMat *ElemsLayerNum",
*else
        "*ElemsNum *ElemsConec(1) *elemsconec(3) *elemsconec(2) *elemsconec(4) *ElemsMat *ElemsLayerNum"
*endif        
*end elems
        ],
*set elems(Quadrilateral)
        "quadrilateral": [
*loop elems
*format "%8i %7i %7i %7i %7i %3i %3i"
*if(nelem != loopvar)
        "*ElemsNum *ElemsConec(1) *elemsconec(2) *elemsconec(3) *elemsconec(4) *ElemsMat *ElemsLayerNum",
*else
        "*ElemsNum *ElemsConec(1) *elemsconec(2) *elemsconec(3) *elemsconec(4) *ElemsMat *ElemsLayerNum"
*endif
*end elems
        ],
*set elems(Triangle)
        "triangle": [
*loop elems
*format "%8i %7i %7i %7i %3i %3i"
*if(nelem != loopvar)
        "*ElemsNum *ElemsConec(1) *elemsconec(2) *elemsconec(3) *ElemsMat *ElemsLayerNum",
*else
        "*ElemsNum *ElemsConec(1) *elemsconec(2) *elemsconec(3) *ElemsMat *ElemsLayerNum"
*endif
*end elems
        ],
*set elems(Linear)
        "line": [
*loop elems
*format "%8i %7i %7i %3i %3i"
*if(nelem != loopvar)
        "*ElemsNum *ElemsConec(1) *ElemsConec(2) *ElemsMat *ElemsLayerNum",
*else
        "*ElemsNum *ElemsConec(1) *ElemsConec(2) *ElemsMat *ElemsLayerNum"
*endif        
*End elems
        ]
    },

*# ----------------------------------------------------------
*# ---------------------- SOURCES ---------------------------
*# ----------------------------------------------------------
*# --- Precounts sources ---
*set var nSources = 0
*set elems(all)
*set Cond Planewave
*loop layers *OnlyInCond
*set var nSources = nSources + 1
*end layers
*loop conditions *nodes
*if(strcasecmp(condName,"Generator_on_line")==0)
*loop nodes *OnlyInCond
*set var nSources = nSources + 1
*end nodes
*endif
*end conditions
*Set Cond Source_on_line *bodyElements
*loop elems *OnlyInCond
*set var nSources = nSources + 1
*end elems
*loop conditions *bodyElements
*if(strcasecmp(condName,"Waveguide_port")==0)
*loop elems *onlyInCond
*set var nSources = nSources + 1
*endif 
*end conditions
*loop conditions *bodyElements
*if(strcasecmp(condName,"TEM_port"))
*set var nSources = nSources + 1
*loop elems *onlyInCond
*end elems
*endif 
*end conditions
*# ----------------------------------------------------------
*set var sourceNum = 0
    "sources": [
*set elems(all)
*Set Cond Planewave
*loop layers *OnlyInCond
        {
            "sourceType": "planewave", 
            "definitionMode": "*cond(Definition_mode)",
*if(strcmp(cond(Definition_mode),"by_vectors")==0)
            "directionVector": "{*cond(Direction_vector)}", 
            "polarizationVector": "{*cond(Polarization_vector)}",
*elseif(strcmp(cond(Definition_mode),"by_angles")==0)
            "directionTheta": *cond(Direction_theta),
            "directionPhi": *cond(Direction_phi),
            "polarizationAlpha": *cond(Polarization_alpha),
            "polarizationBeta": *cond(Polarization_beta),
*else
            "numberOfRandomPlanewaves": *cond(Number_of_random_planewaves),
            "relativeVariationOfRandomDelay": *cond(Relative_variation_of_random_delay),
*endif            
*include includes/magnitude.bas
            "layerName": "*layerName",
            "layerBox": "*tcl(GiD_Info layer -bbox -use geometry *layerName)",
*set var sourceNum = sourceNum + 1
*if(sourceNum == nSources) 
        }
*else
        },
*endif
*end layers
*# ----------------------------------------------------------
*loop conditions *nodes
*if(strcasecmp(condName,"Generator_on_line")==0)
*loop nodes *OnlyInCond
        {
            "sourceType": "generator",
            "type": "*cond(Type)",
*include includes/magnitude.bas
            "coordIds": [ *NodesNum ]
*if(sourceNum == nSources) 
        }
*else
        },
*endif
*end nodes
*endif
*end conditions
*# ----------------------------------------------------------
*Set Cond Source_on_line *bodyElements
*set var HEADER=0
*loop elems *OnlyInCond
*if(HEADER == 0)
*set var HEADER=1
        {
            "sourceType": "sourceOnLine",
            "type": "*cond(Type)", 
            "hardness": "*cond(Hardness)",
*include includes/magnitude.bas
            "defined": "OnElements",
            "elemIds": [
*endif
*if(loopVar != condNumEntities)
                *elemsNum,
*else
                *elemsNum
*endif
*end elems
            ]
        },
*# ----------------------------------------------------------
*loop conditions *bodyElements
*if(strcasecmp(condName,"Waveguide_port")==0&&condNumEntities>0)
*set var HEADER = 0
*loop elems *onlyInCond
*if(HEADER == 0)
*set var HEADER = 1
        {
            "sourceType": "waveguidePort",
            "shape": "*cond(Shape)",
*include includes/magnitude.bas
            "excitationMode": "*cond(Mode)",
            "firstMode":  *cond(FirstMode),
            "secondMode": *cond(SecondMode),
            "elemIds": [
*endif
*if(loopVar != condNumEntities)
                *ElemsNum,
*else
                *ElemsNum
*endif
*end elems
            ]
        },
*endif 
*end conditions
*# ----------------------------------------------------------
*loop conditions *bodyElements
*if(strcasecmp(condName,"TEM_port")==0&&condNumEntities>0)
*set var ENTITY = 0
*loop elems *onlyInCond
*if(ENTITY == 0)
*set var ENTITY = cond(entityId,INT)
        {
            "sourceType": "temPort",
*include includes/magnitude.bas
            "origin": "*tcl(GiD_Info Coordinates *cond(Origin) geometry)",
            "innerRadius": *cond(Inner_radius),
            "outerRadius": *cond(Outer_radius),
            "excitationMode": "*cond(Mode)",
            "elemIds": [
*endif
*if(loopVar != condNumEntities)
                *ElemsNum,
*else
                *ElemsNum
*endif
*end elems
            ]
        }, 
*endif 
*end conditions
    ],

*# ----------------------------------------------------------
*# ------------------ OUTPUT REQUESTS -----------------------
*# ----------------------------------------------------------
    "outputRequests": [
*# ----------------------------------------------------------
*tcl(semba::writeOutputRequestBAS OutRq_on_point)
*tcl(semba::writeOutputRequestBAS OutRq_on_line)
*tcl(semba::writeOutputRequestBAS OutRq_on_surface)
*# ----------------------------------------------------------
*Set cond OutRq_on_layer
*if(CondNumEntities(int)>0)
        {
            "gidOutputType": "OutRq_on_layer",
*loop layers *OnlyInCond
            "name": "*cond(Name)",
            "type": "*cond(Type)",
*include includes/domain.bas
            "box": "{*tcl(GiD_Info layer -bbox -use geometry *layerName)}"
*end layers
        },
*end if
*# ----------------------------------------------------------
*tcl(semba::writeOutputRequestBulkCurrentBAS Bulk_current_on_surface)
*# ----------------------------------------------------------
*Set cond Bulk_current_on_layer
*if(CondNumEntities(int)>0)
        {
            "gidOutputType": "Bulk_current_on_layer",
*loop layers *OnlyInCond
            "name": "*cond(Name)",
            "type": "*cond(Type)",
*include includes/domain.bas
            "direction": "{*cond(Direction)}",
            "skip": *cond(Skip),
            "box": {*tcl(GiD_Info layer -bbox -use geometry *layerName)}
*end layers
        },
*end if
*# ----------------------------------------------------------
*Set cond Far_field
*if(CondNumEntities(int)>0)
        { 
            "gidOutputType": "Far_field",
*loop layers *OnlyInCond
            "name": "*cond(Name)",
            "type": "*cond(Type)",
*include includes/domain.bas
            "box": "{*tcl(GiD_Info layer -bbox -use geometry *layerName)}",
            "farPoints": {
                "initialTheta": *cond(Initial_theta),
                "finalTheta":   *cond(Final_theta),
                "stepTheta":    *cond(Step_theta),
                "initialPhi":   *cond(Initial_phi),
                "finalPhi":     *cond(Final_phi),
                "stepPhi":      *cond(Step_phi)
            }
*end layers
        },
*end if
    ]
}