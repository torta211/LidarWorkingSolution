import Ccode
import numpy as np
import open3d as o3d
import time
import matplotlib.pyplot as plt

#uncomment this section to get the "press a to show and process new frame"-style program

##bins = range(0, 401)

##distinct_colors = np.array([[0, 0, 0],
##                            [1, 1, 1],
##                            [1, 0, 0],
##                            [0, 1, 0],
##                            [0, 0, 1],
##                            [0, 1, 1],
##                            [1, 0, 1],
##                            [1, 1, 0],
##                            [0.75, 0.75, 0.75],
##                            [0.5, 0.5, 0.5],
##                            [0.5, 0, 0],
##                            [0.5, 0.5, 0],
##                            [0, 0.5, 0],
##                            [0.5, 0, 0.5],
##                            [0, 0.5, 0.5],
##                            [0, 0, 0.5]])

##def display_new_frame(erebere):
##    frame_in_np = np.array(lidar.retrieve())
##    frame_in_np = frame_in_np[frame_in_np[:, 3] != 0]

##    dist = frame_in_np[:, 3]
##    plt.cla()
##    (n, _, _) = plt.hist(dist, bins)
##    plt.pause(0.01)

##    colors = np.zeros([frame_in_np.shape[0], 3])
##    print(np.argmax(n))
##    colors[frame_in_np[:, 3] < np.argmax(n) + 5] = distinct_colors[4, :]

##    pcd_disp.points = o3d.utility.Vector3dVector(frame_in_np[:, 0:3])
##    pcd_disp.colors = o3d.utility.Vector3dVector(colors)
##    vis.update_geometry()
##    vis.update_renderer()
##    vis.poll_events()


### program
##lidar = Ccode.VeloParser()
##lidar.open_live(2)

##vis = o3d.visualization.VisualizerWithKeyCallback()
##vis.register_key_callback(65, display_new_frame)
##vis.create_window()

##pcd_disp = o3d.geometry.PointCloud()
##frame_in_np = np.array(lidar.retrieve())
##pcd_disp.points = o3d.utility.Vector3dVector(frame_in_np[:, 0:3])
##vis.add_geometry(pcd_disp)

##vis.update_geometry()
##vis.update_renderer()
##vis.poll_events()

##vis.run()


#real-time version (flawed) (problem with getting not full sized frames)
import multiprocessing as mp
from queue import Empty as queue_empty

class Viewer(object):
  def __init__(self):
    self.a = 1
    self.q = mp.Queue()

  def worker(self):
    lidar = Ccode.VeloParser()
    lidar.open_live(2)
    for i in range(0, 10000):
        frame = lidar.retrieve()
        print("frame arrived, size is: ", len(frame))
        frame_np = np.array(frame)[:, 0:3]
        self.q.put(frame_np, block=True)
        print("queued frames waiting for display: ", self.q.qsize())
    self.q.put(None)

  def run(self):
    pcd_disp = o3d.geometry.PointCloud()
    vis = o3d.visualization.Visualizer()
    vis.create_window()

    first_frame_in_np = np.array(Ccode.get_frame())[:, 0:3]
    pcd_disp.points = o3d.utility.Vector3dVector(first_frame_in_np)

    vis.add_geometry(pcd_disp)
    vis.poll_events()
    vis.update_renderer()

    p = mp.Process(target=self.worker)
    p.start()

    keep_running = True

    while keep_running:
      try:
        frame_in_np = self.q.get(block=True)
        if frame_in_np is not None:
          print("displaying new frame of size: ", frame_in_np.shape[0])
          print("")
          pcd_disp.points = o3d.utility.Vector3dVector(frame_in_np)
        else:
          print('got poison. dying')
          keep_running = False
        vis.update_geometry()
      except queue_empty:
        pass

      vis.update_renderer()
      keep_running = keep_running and vis.poll_events()

    vis.destroy_window()
    p.join()

if __name__ == '__main__':
  v = Viewer()
  v.run()

