#!/usr/bin/env python3
import csv
import heapq
import math
from collections import defaultdict

import tqdm


def vector_diff(a, b):
    """
    Vector from A to B
    """
    return b[0] - a[0], b[1] - a[1]


def unit_vector(v):
    m = math.sqrt(sum(c**2 for c in v))
    if m == 0:
        return [0 for _ in v]
    return [c / m for c in v]


def main():
    csv_file = "TaxiData.csv"

    data_per_taxi = defaultdict(list)

    with open(csv_file, "r") as fp:
        cfp = csv.reader(fp)
        next(cfp)

        for line in cfp:
            taxi_id, time, latitude, longitude, occupancy_status, speed = line
            taxi_id = int(taxi_id)
            latitude = float(latitude)
            longitude = float(longitude)
            speed = float(speed)

            data_per_taxi[taxi_id].append((time, latitude, longitude, speed))

    new_data_per_taxi = defaultdict(list)

    for k in tqdm.tqdm(data_per_taxi.keys()):
        data_per_taxi[k].sort()

        new_k_data = []
        prev_pos = None
        taxi_data = data_per_taxi[k]
        for i, (time, latitude, longitude, speed) in enumerate(taxi_data):
            new_prev = (latitude, longitude)

            if prev_pos is None:
                prev_pos = (latitude, longitude)

                if i != len(taxi_data) - 1:
                    latitude = taxi_data[i + 1][1]
                    longitude = taxi_data[i + 1][2]

            cp = (latitude, longitude)

            vd = vector_diff(prev_pos, cp)
            vd = unit_vector(vd)

            # Longitude is X, latitude is Y
            speed_x = vd[1] * speed
            speed_y = vd[0] * speed
            new_k_data.append((k, time, latitude, longitude, speed, speed_x, speed_y))

            prev_pos = new_prev

        new_data_per_taxi[k] = new_k_data

    out_name = "TaxiData-sorted.csv"
    with open(out_name, "w") as fp:
        writer = csv.writer(fp)
        writer.writerow(
            ["taxi_id", "time", "latitude", "longitude", "speed", "speed_x", "speed_y"]
        )
        for tup in heapq.merge(*list(new_data_per_taxi.values()), key=lambda t: t[1]):
            writer.writerow(tup)


if __name__ == "__main__":
    main()
