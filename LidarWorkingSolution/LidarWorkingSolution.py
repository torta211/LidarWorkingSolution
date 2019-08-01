import Ccode
import numpy as np
import open3d as o3d
import multiprocessing as mp
import time
from queue import Empty as queue_empty

lidar = Ccode.VeloParser()

class Viewer(object):
  def __init__(self):
    self.q = mp.Queue()

  def worker(self, queue):
    for i in range(0, 100):
        frame = lidar.read_frame()
        frame_np = np.array(frame)
        queue.put(frame_np)
    queue.put(None)

  def run(self):
    pcd_disp = o3d.geometry.PointCloud()
    pcd = o3d.geometry.PointCloud()
    vis = o3d.visualization.Visualizer()
    vis.create_window()

    self.lidar.open_live(2)
    frame_f = self.lidar.read_frame();
    print(type(frame_f))
    print(type(frame_f[0]))
    frame_in_np_f = np.array(frame_f)
    print(frame_in_np_f.shape)
    pcd_disp.points = o3d.utility.Vector3dVector(frame_in_np_f)

    vis.add_geometry(pcd_disp)
    vis.poll_events()
    vis.update_renderer()

    p = mp.Process(target=self.worker, args=(self.q,))
    p.start()

    keep_running = True

    while keep_running:
      try:
        frame_in_np = self.q.get(block=False)
        if frame_in_np is not None:
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

