# OpenSEMBA
# Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
#                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
#                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
#                    Daniel Mateos Romero            (damarro@semba.guru)
#
# This file is part of OpenSEMBA.
#
# OpenSEMBA is free software: you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

proc InitGIDProject { dir } {
    set ::semba::_dir $dir

	# Reads .xml
	if { [info procs ::ReadProblemtypeXml] == "" } {
	WarnWin [=  "This GiD version is too old, get the last available version"]
	return 1
    }
    
    set xmlfile [file join $dir semba.xml]
    set data [ReadProblemtypeXml $xmlfile Infoproblemtype {Version EngineVersion MinimumGiDVersion Name Homepage Maintainer}]
    if { $data == "" } {
	if { [ file exists $xmlfile]} {
	    WarnWinText [= "Couldn't read file %s" $xmlfile]
	} else {
	    WarnWinText [= "Configuration file %s not found" $xmlfile]
	}
	return 1
    }
	
	array set problemtype_local $data
    set semba::VersionNumber $problemtype_local(Version)
	set semba::EngineVersion $problemtype_local(EngineVersion)
	set semba::ProgramName $problemtype_local(Name)
	set semba::Web $problemtype_local(Homepage)
	set semba::Maintainer $problemtype_local(Maintainer)

    # Check the required GiD version   
    if { [::GidUtils::VersionCmp $problemtype_local(MinimumGiDVersion)]  < 0 } {
		WarnWinText [= "Error: This interface requires GiD %s or later" $problemtype_local(MinimumGiDVersion)].
    }

    # Shows splash window at start
    #semba::Splash
    # Checks packages
    package require treectrl
    package require math
    # Modifies menus available to the user.
    semba::AddToolbar

	GidChangeDataLabel "Interval" ""

	GiDMenu::InsertOption "Help" [list ---] end PREPOST {} "" "" insertafter
    GiDMenu::InsertOption "Help" [list [concat [_ "About"] " " $::semba::ProgramName]...] end PREPOST semba::About "" "" insertafter

    # Update the menu properties
    ::GiDMenu::UpdateMenus
}

proc EndGIDProject {} {
    semba::endToolbars
}

proc LoadGIDProject { filespd } {        
    set pt [GiD_Info project ProblemType]
    if { $pt == "semba" } {
		set filename [file rootname $filespd].xml
		#set model_problemtype_version_number [OpenFoam::ReadXml $filename]            
    }
}

proc BeforeMeshGeneration { elementSize } {
    # This checks that there is a layer called Grid.
    # DBG: WarnWin [GiD_Info layers] 
    #    set gridPresent 0
    #   foreach layer [GiD_Info layers] {
    #if {[string equal -nocase $layer "Grid"]} {
    # DBG: WarnWin [= "Grid layer has been detected"]
    #        set gridPresent 0
    # Does further checkings on grid.
    # TODO Further checkings on grid before mesh generation.
    # Turns off grid layer.
    # TODO For performance, on mesh visualization, grid is turned off.
    #}
    #    }
    #    if {$gridPresent == 0} {
    #            return $elementSize
    #    } else {
    #            WarnWin [= "Mesh generation has been canceled because there is no Grid layer present."]
    #            return "-cancel-"
    #    }
}

namespace eval semba {
    variable ProgramName ;
    variable VersionNumber ; # Read from .xml
	variable EngineVersion
    variable Web ;
	variable Maintainer ;
    variable _dir ;#path to the problemtype
}

proc semba::Splash { } {
    variable ProgramName
    variable VersionNumber
	variable EngineVersion
	variable Maintainer
	variable _dir

    set prev_splash_state [GiD_Set SplashWindow]
    GiD_Set SplashWindow 1 ;
    set off_x 10
    set fnt "Sans 10"
    if { $::tcl_platform(platform) == "windows" } {
	set fnt "verdana 10"
	set off_x 10
    }
    ::GidUtils::Splash [file join $_dir images splash.png] .splash 1 \
    [list "$::semba::ProgramName v$::semba::VersionNumber - $::semba::EngineVersion\n Developed by $Maintainer \n Contact and support: $::semba::Web \n Copyright 2015. All rights reserved." $off_x 10]
    set new_splash [ ::GidUtils::VersionCmp 11.1.6d]
    if { [ winfo exists .splash.lv] && ( $new_splash < 0)} {
	    .splash.lv configure -font $fnt -background white -foreground black \
		-relief solid -borderwidth 1 -padx 12 -pady 3
	    update
    }
    GiD_Set SplashWindow $prev_splash_state
}

proc semba::About { } {
    variable ProgramName
    variable VersionNumber
	variable EngineVersion
	variable Maintainer
	variable _dir

    set prev_splash_state [GiD_Set SplashWindow]
    GiD_Set SplashWindow 1
    set off_x 10
    set fnt "Sans 10"
    if { $::tcl_platform(platform) == "windows" } {
	set fnt "verdana 10"
	set off_x 10
    }
    ::GidUtils::Splash [file join $_dir images splash.png] .splash 0 \
	[list "$::semba::ProgramName v$::semba::VersionNumber - $::semba::EngineVersion\n Developed by $Maintainer \n Contact and support: $::semba::Web \n Copyright 2015. All rights reserved." $off_x 10]

    set new_splash [ ::GidUtils::VersionCmp 11.1.6d]
    if { [ winfo exists .splash.lv] &&  ( $new_splash < 0)} {
	    .splash.lv configure -font $fnt -background white -foreground black \
		-relief solid -borderwidth 1 -padx 12 -pady 3
	    update
    }
    GiD_Set SplashWindow $prev_splash_state
}


proc semba::AddToolbar { { type "DEFAULT INSIDELEFT"} } {
    variable _dir
    
    global sembaBitmapsNames 
    global sembaBitmapsCommands
    global sembaBitmapsHelp
    global ProblemTypePriv
    global GIDDEFAULT
    # List of bitmaps.
    set sembaBitmapsNames(0) [ list \
		                                         images/icons/data.gif \
		                     images/icons/meshingConfiguration.gif \
		                                         images/icons/boundingbox.gif \
		                                         images/icons/meshing.gif \
		                                         images/icons/predefinedMaterials.gif \
		                                         images/icons/materials.gif \
		                     images/icons/advancedMaterials.gif \
		                     images/icons/wires.gif \
		                     images/icons/sources.gif \
		                     images/icons/requests.gif \
		                                         images/icons/structured.gif \
		                    ]
    # List of commands.
    set sembaBitmapsCommands(0) [list \
		                       [list -np- GidOpenProblemData "Solver_options"] \
		                       [list -np- GidOpenProblemData "Mesher_options"] \
		                                           [list -np- semba::OpenBoundingBoxData] \
		                                           [list -np- GidOpenConditions "Meshing"] \
		                       [list -np- GidOpenMaterials "Predefined_materials"] \
		                       [list -np- GidOpenMaterials "Basic_materials"] \
		                       [list -np- GidOpenMaterials "Advanced_materials"] \
		                       [list -np- GidOpenMaterials "Wires"] \
		                       [list -np- GidOpenConditions "Electromagnetic_sources"] \
		                       [list -np- GidOpenConditions "Output_Requests"] \
		                                           [list -np- semba::generateStructuredMesh $_dir] \
		                      ]
    
    set sembaBitmapsHelp(0) [list \
		                   [= "Solver options"] \
		                   [= "Mesher options"] \
		                                   [= "Create bounding box"] \
		                                   [= "Grid"] \
		                   [= "Predefined Materials"] \
		                   [= "Basic Materials"] \
		                   [= "Advanced Materials"] \
		                   [= "Wires and terminations"] \
		                   [= "Electromagnetic sources"] \
		                   [= "Output Requests"] \
		                   [= "Generate .smb"] \
		                  ]
    # prefix values:
    # Pre        Only active in the preprocessor
    # Post       Only active in the postprocessor
    # PrePost    Active Always
    set prefix Pre
    set ProblemTypePriv(toolbarwin) [ \
		                          CreateOtherBitmaps sembaBar [= "Semba bar"] \
		                          sembaBitmapsNames \
		                          sembaBitmapsCommands \
		                          sembaBitmapsHelp $_dir semba::AddToolbar $type $prefix \
		                         ]
    # AddNewToolbar "semba bar" ${prefix}sembaBarWindowGeom \
	# "semba::MyBitmaps [list $dir]" [= "semba bar"]
    AddNewToolbar "semba bar" ${prefix}sembaBarWindowGeom \
	semba::AddToolbar [= "Semba bar"]
}

proc semba::endToolbars {} {
    global ProblemTypePriv
    ReleaseToolbar "semba bar"
    catch { destroy $ProblemTypePriv(toolbarwin) }
}

proc semba::generateStructuredMesh { dir } {
    global GIDDEFAULT 
	if {[GiD_MustRemeshFlag get]} {
		WarnWin [= "WARNING: Some changes have been made to geometry, conditions or materials. Mesh should probably be generated again."]
	}
    set modelDir [GiD_Info Project ModelName].gid/
    set modelName [file tail [GiD_Info Project ModelName] ]
    # Outputs .stl from geometry.
    GiD_Process Mescape Files WriteForBAS \
	[file join $dir/semba.bas] \
	[file join $modelDir $modelName.smb]
#    # Launches script.
#    exec $dir/scripts/generateNFDE.sh \
#        $dir $modelDir $modelName \
#        > $modelDir/log.generateNFDE
}

proc semba::createBoundingBox {xMaxPad yMaxPad zMaxPad xMinPad yMinPad zMinPad} {

    set layers [GiD_Info layers]
    
#     set positions [join [GiD_Info layer -bbox -use geometry [lindex $layers 0]]]
    set positions [join [GiD_Info layer -bbox -use geometry]]
    set xmax [lindex $positions 0]
    set ymax [lindex $positions 1]
    set zmax [lindex $positions 2]
    set xmin [lindex $positions 3]
    set ymin [lindex $positions 4]
    set zmin [lindex $positions 5]
    
#     foreach layer $layers {
#         set positions [join [GiD_Info layer -bbox -use geometry $layer]]
#         set xmax [::math::max $xmax [lindex $positions 0]]
#         set ymax [::math::max $ymax [lindex $positions 1]]
#         set zmax [::math::max $zmax [lindex $positions 2]]
#         set xmin [::math::min $xmin [lindex $positions 3]]
#         set ymin [::math::min $ymin [lindex $positions 4]]
#         set zmin [::math::min $zmin [lindex $positions 5]]
#     }
    
    set xmax [expr {$xmax + $xMaxPad}]
    set ymax [expr {$ymax + $yMaxPad}]
    set zmax [expr {$zmax + $zMaxPad}]
    set xmin [expr {$xmin - $xMinPad}]
    set ymin [expr {$ymin - $yMinPad}]
    set zmin [expr {$zmin - $zMinPad}]
	
	# Stores Id of last created surface.
	set oldId [GiD_Info Geometry MaxNumSurfaces]    
	# Creates rectangle in base.
    GiD_Process Mescape Geometry Create Object Rectangle $xmin $ymin $zmin $xmax $ymax $zmin
	set newId [GiD_Info Geometry MaxNumSurfaces]    

	# Disables dialog to create new point.
	set statusCreateAlwaysNewPoint [GiD_Set CreateAlwaysNewPoint]
	GiD_Set CreateAlwaysNewPoint 1

	if {$newId == [expr $oldId + 1]} {
	    # Extrudes rectangle.
	    GiD_Process Mescape Utilities Copy Surfaces Duplicate DoExtrude Surfaces \
		         Translation FNoJoin 0.0 0.0 $zmin FNoJoin 0.0 0.0 $zmax end escape   
	} else {
#                 # Determines if there is a surface containing all the points.
#                 set surfaceFound 0
#                 foreach surf [GiD_Geometry list surface 1:end] {
#                         set allInside true
#                         set allInside [expr $allInside && [GiD_Info IsPointInside Surface $surf {$xmin $ymin $zmin}]]
#                         set allInside [expr $allInside && [GiD_Info IsPointInside Surface $surf {$xmin $ymax $zmin}]]
#                         set allInside [expr $allInside && [GiD_Info IsPointInside Surface $surf {$xmax $ymin $zmin}]]
#                         set allInside [expr $allInside && [GiD_Info IsPointInside Surface $surf {$xmax $ymax $zmin}]]
#                         if {$allInside} {
#                                 set surfaceFound $surf
#                         }
#                 }
#                 if {$surfaceFound != 0} {
#                         GiD_Process Mescape Utilities Copy Surfaces Duplicate DoExtrude Surfaces \
#                                  Translation FNoJoin 0.0 0.0 $zmin FNoJoin 0.0 0.0 $zmax $surfaceFound escape   
#                 } else {
		        GiD_Process Mescape
		        WarnWin [=  "Unable to create automatic bounding box"]
#                 }
	}

	# Restores status of createAlwaysNewPoint.
	GiD_Set CreateAlwaysNewPoint $statusCreateAlwaysNewPoint
}


proc semba::OpenBoundingBoxData { } {
	global BoundingBoxDataPriv

	if { ![info exists BoundingBoxDataPriv(samesizes)] } {
		set BoundingBoxDataPriv(samesizes) 1
	}        
	foreach axe {xMax yMax zMax xMin yMin zMin} {
		if { ![info exists BoundingBoxDataPriv(size,$axe)] } {
		        set BoundingBoxDataPriv(size,$axe) 0.0
		}
	}

	set w .gid.boundingboxdata
	if { [winfo exists $w] } return

	InitWindow $w [_ "Automatic Bounding box"] BoundingBoxDataWindowGeom OpenBoundingBoxData
	if { ![winfo exists $w] } return ;# windows disabled || usemorewindows == 0

	ttk::frame $w.f0

	if { ![info exists BoundingBoxDataPriv(samesizes)] } { set BoundingBoxDataPriv(samesizes) 0 }
	ttk::checkbutton $w.chk -text [_ "Same sizes"] -variable BoundingBoxDataPriv(samesizes)
	foreach axe {xMax yMax zMax xMin yMin zMin} {
		ttk::label $w.f0.l$axe -text [concat [_ "Pad"] $axe]
		ttk::entry $w.f0.e$axe -textvariable BoundingBoxDataPriv(size,$axe)
		GidHelp $w.f0.e$axe [_ "Size that bounding box will be padded"]
		bind $w.f0.e$axe <Return> "semba::OnChangeBoundingBoxSize $axe"
		bind $w.f0.e$axe <FocusOut> "semba::OnChangeBoundingBoxSize $axe"
	}
	ttk::label $w.f0.lcomment -text [_ ""]
   
	set def_back [$w cget -background]

	ttk::frame $w.frmButtons -style BottomFrame.TFrame
	ttk::button $w.frmButtons.btnApply -text [_ "Apply"] -command [list semba::ApplyBoundingBoxData $w] \
		-underline 0 -style BottomFrame.TButton
#        ttk::button $w.frmButtons.btnclose -text [_ "Close"] -command {GiD_Process escape} \
#                -underline 0 -style BottomFrame.TButton

	grid $w.chk -sticky w -columnspan 2
	foreach axe {xMin yMin zMin xMax yMax zMax} {
		grid $w.f0.l$axe $w.f0.e$axe -sticky ew -padx 2
		grid configure $w.f0.l$axe -sticky e
	} 
	grid $w.f0.lcomment -columnspan 2
	grid $w.f0 -sticky nsew   
	grid rowconfigure $w.f0 6 -weight 1
	grid columnconfigure $w.f0 1 -weight 1   

	grid $w.frmButtons -sticky ews -columnspan 7
	if { $::tcl_version >= 8.5 } { grid anchor $w.frmButtons center }

#        grid $w.frmButtons.btnApply $w.frmButtons.btnclose -padx 5 -pady 6
	grid $w.frmButtons.btnApply -padx 5 -pady 6
	grid columnconfigure $w 0 -weight 1
	grid rowconfigure $w 1 -weight 1

	focus $w.frmButtons.btnApply

#        bind $w <Alt-c> "$w.frmButtons.btnclose invoke"
#        bind $w <Escape> "$w.frmButtons.btnclose invoke"
	bind $w <Return> "$w.frmButtons.btnApply invoke"
	#bind $w <Destroy> [list +DestroyBoundingBoxData %W $w] ;# + to add to previous script

	wm minsize $w 140 140

	if { $::tcl_platform(os) != "Darwin"} {
		#catch { tkwait visibility $w }
		update
	} else {
		update
	}
	grab set $w
}

proc semba::OnChangeBoundingBoxSize { i } {
	global BoundingBoxDataPriv
	if { $BoundingBoxDataPriv(samesizes) } {
		foreach axe {xMax yMax zMax xMin yMin zMin} {
		        if { $axe == $i } continue
		        set BoundingBoxDataPriv(size,$axe) $BoundingBoxDataPriv(size,$i)
		}
	}
}

proc semba::ApplyBoundingBoxData { w } {
	global BoundingBoxDataPriv

	foreach axe {xMax yMax zMax xMin yMin zMin} {
		if { ![info exists BoundingBoxDataPriv(size,$axe)] } {
		        set BoundingBoxDataPriv(size,$axe) [string trim $BoundingBoxDataPriv(size,$axe)]
		}
	}

    semba::createBoundingBox $BoundingBoxDataPriv(size,xMax) $BoundingBoxDataPriv(size,yMax) $BoundingBoxDataPriv(size,zMax) $BoundingBoxDataPriv(size,xMin) $BoundingBoxDataPriv(size,yMin) $BoundingBoxDataPriv(size,zMin)
       
	grab release $w
	#destroy $w
	#FinishButton $w $w.frmButtons [_ "Press 'Finish' to end selection"] "" disableall [GiD_Set SmallWinSelecting]
}

proc semba::CloseBoundingBoxData { } {
	set w .gid.boundingboxdata
	if { [winfo exists $w] } {        
		destroy $w
	}
}

