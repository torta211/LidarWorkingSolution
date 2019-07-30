from Ccode import get_frame
import numpy as np
import open3d as o3d

for i in range(0, 1000):
    frame = get_frame(10);
    print("frame arrived to Python")

    frame_in_np = np.array(frame)
    print(frame_in_np.shape)

    pcd = o3d.geometry.PointCloud()
    pcd.points = o3d.utility.Vector3dVector(frame_in_np)

    print("starting to display plot")

    o3d.visualization.draw_geometries([pcd])

