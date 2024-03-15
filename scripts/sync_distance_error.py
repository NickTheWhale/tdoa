C = 3e8              # m/s
sync_interval = 10e-3 # 10ms
stability = 3 / 1e6  # ppm

drift = sync_interval * stability
distance_error = C * drift


print(f'drift: {drift*1e9:.2f} (ns)')
print(f'distance error {distance_error*1e3:.2f} (mm)')