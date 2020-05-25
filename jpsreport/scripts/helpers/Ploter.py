import os
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from mpl_toolkits.axes_grid1 import make_axes_locatable
from matplotlib.patches import Polygon as ppolygon # polygons

import numpy as np
import glob
from xml.dom.minidom import parse
from Utilities import read_obstacle
from Utilities import read_subroom_walls
from Utilities import geo_limits

def plot_nt(measurement_id, files):
    """plot N(t) curves (Method A)
   
    :param measurement_id: measurement area id
    :param files: list of files -> Fundamental_Diagram/FlowVelocity/Flow_NT_*.dat

    """
    legend = False #show legend if more than one measurement_id is plotted
    fig = plt.figure(dpi=300)
    ax1 = fig.add_subplot(111, aspect='auto')
    for f in files:
        data = np.loadtxt(f)
        Id = int(f.split("id_")[-1].split(".")[0])
        if measurement_id == -1:
            plt.plot(data[:, 0], data[:, 1], label="{}".format(Id))
            legend = True
            continue
        
        if Id == measurement_id:
            plt.plot(data[:, 0], data[:, 1])

    plt.xlabel("t")
    plt.ylabel("N")
    if legend:
        plt.legend()

    figname = "Nt_{}.png".format(measurement_id)
    plt.savefig(figname)
    print("---> ", figname)

def plot_profiles(Id, field_dir, geo_filename, beginsteady=None, endsteady=None):
    """ Plot density, velocity and flow 

    Note: J = rho*v

    :param Id: measurement area
    :param field_dir: 
    :param beginsteady: 
    :param endsteady:
    :param geo_filename: 
    :returns: 
    :rtype: 

    """

        # for j in range(beginsteady, endsteady):
        # density_file = os.path.join(density_files_path, "Prf_d_%s_id_1_%.5d.dat" %(nametraj, j))
        
    density_files = os.path.join(field_dir, "density",  "Prf_*id_{}_*".format(Id))
    velocity_files = os.path.join(field_dir, "velocity",  "Prf_*id_{}_*".format(Id))
    v_Voronoi = glob.glob(velocity_files)
    f_Voronoi = glob.glob(density_files)
    # get frames
    frames = []
    for f in v_Voronoi:
        frame = int(f.split("_")[-1].split(".")[0]) #Prf_d_*_id_%d_%.5d.dat)
        frames.append(frame)

    frames = sorted(frames)
    # correct arguments if necessary
    if beginsteady is None:
        beginsteady = frames[0]
    elif beginsteady < frames[0]:
        beginsteady = frames[0]
        
    if endsteady is None:
        endsteady = frames[-1]
    elif endsteady > frames[-1]:
        endsteady = frames[-1]

    
    beginsteady -= frames[0]
    endsteady -= frames[0]
    
    # get the shape of the matrices
    shape = np.shape(np.loadtxt(f_Voronoi[0]))
    density = np.zeros(shape)
    velocity = np.zeros(shape)
    xml_datei = open(geo_filename, "r")
    geo_xml = parse(xml_datei)
    xml_datei.close()
    geominX, geomaxX, geominY, geomaxY = geo_limits(geo_xml)

    geometry_wall = read_subroom_walls(geo_xml)
    geometry_obst = read_obstacle(geo_xml)
#  -------- density
    for density_file in f_Voronoi[beginsteady:endsteady+1]:
        if os.path.exists(density_file):    
            density += np.loadtxt(density_file)

    density = density / (endsteady-beginsteady)
#  --------- velocity
    for velocity_file in v_Voronoi[beginsteady:endsteady+1]:
        if os.path.exists(velocity_file):    
            velocity += np.loadtxt(velocity_file)

    velocity = velocity / (endsteady-beginsteady)
    flow = density * velocity
   
 # plot
    figs, axs = plt.subplots(3, 1)
    axs[0].set_aspect('equal')
    
    for g in geometry_obst.keys():
        axs[0].add_patch(ppolygon(geometry_obst[g], color='white'))
        axs[1].add_patch(ppolygon(geometry_obst[g], color='white'))
        axs[2].add_patch(ppolygon(geometry_obst[g], color='white'))

    for gw in geometry_wall.keys():
        axs[0].plot(geometry_wall[gw][:, 0], geometry_wall[gw][:, 1], color='white', lw=1)
        axs[1].plot(geometry_wall[gw][:, 0], geometry_wall[gw][:, 1], color='white', lw=1)
        axs[2].plot(geometry_wall[gw][:, 0], geometry_wall[gw][:, 1], color='white', lw=1)

    im1 = axs[0].imshow(density, 
                    cmap=cm.jet,
                    interpolation='nearest', origin='lower',
                    vmin=0, vmax=4, #np.amax(density)
                    extent=[geominX, geomaxX, geominY, geomaxY]) 
    
    im2 = axs[1].imshow(velocity, 
                    cmap=cm.jet,
                    interpolation='nearest', origin='lower',
                    vmin=0, vmax=1.2, #np.amax(velocity)
                    extent=[geominX, geomaxX, geominY, geomaxY])
    
    im3 = axs[2].imshow(flow,
                    cmap=cm.jet, interpolation='nearest', origin='lower',
                    vmin=0, vmax=2, #np.amax(flow)
                    extent=[geominX, geomaxX, geominY, geomaxY])
    
    axs[0].set_xlabel("x [m]")
    axs[0].set_ylabel("y [m]")
    divider1 = make_axes_locatable(axs[0])
    cax1 = divider1.append_axes("right", size="3.5%", pad=0.3)
    cb1 = plt.colorbar(im1, cax=cax1)
    cb1.set_label('Density [$m^{-2}$]')
    
    axs[1].set_xlabel("x [m]")
    axs[1].set_ylabel("y [m]")
    divider2 = make_axes_locatable(axs[1])
    cax2 = divider2.append_axes("right", size="3.5%", pad=0.3)
    cb2 = plt.colorbar(im2, cax=cax2)
    cb2.set_label('Velocity [$m/s$]')
    
    axs[2].set_xlabel("x [m]")
    axs[2].set_ylabel("y [m]")
    divider3 = make_axes_locatable(axs[2])
    cax3 = divider3.append_axes("right", size="3.5%", pad=0.3)
    cb3 = plt.colorbar(im3, cax=cax3)
    cb3.set_label('Flow [$1/m \cdot s$]')
    plt.tight_layout()
    figname = "profiles_{}.png".format(Id) # todo: depend on trajectories name
    plt.plot(figname)
    plt.savefig(figname)
    print("--->", figname)
