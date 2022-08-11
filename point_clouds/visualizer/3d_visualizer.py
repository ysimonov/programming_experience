import numpy as np
import pye57
import open3d as o3d

input_data = "data/Westham.e57"

if __name__ == '__main__':

    data = pye57.E57(input_data)

    point_cloud = data.read_scan(
        index=0, 
        intensity=True,
        colors=True,
        ignore_missing_fields=True
    )

    points_3d = np.vstack((
        np.asarray(point_cloud['cartesianX']),
        np.asarray(point_cloud['cartesianY']),
        np.asarray(point_cloud['cartesianZ'])
    )).transpose()

    colors_rgb = np.vstack((
        np.asarray(point_cloud['colorRed']),
        np.asarray(point_cloud['colorGreen']),
        np.asarray(point_cloud['colorBlue'])
    )).transpose()

    intensities = np.asarray(point_cloud['intensity'])

    step_size = 4
    sampled_points_3d = points_3d[::step_size]
    sampled_colors_rgb = colors_rgb[::step_size]
    sampled_intensities = intensities[::step_size]

    # pass xyz to Open3D
    pcd = o3d.geometry.PointCloud()
    pcd.points = o3d.utility.Vector3dVector(sampled_points_3d)
    pcd.colors = o3d.utility.Vector3dVector(sampled_colors_rgb)
    o3d.visualization.draw_geometries([pcd])