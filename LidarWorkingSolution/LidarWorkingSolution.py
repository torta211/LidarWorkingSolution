from Ccode import get_test_list
import numpy as np
import open3d as o3d


a = get_test_list(10);

pcl_to_disp = np.array(a)


o3d.visualization.draw_geometries([pcl_to_disp])

