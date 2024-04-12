from bagpy import bagreader
import pandas as pd
import matplotlib.pyplot as plt
#----------------------------------------------.bag파일과 시간축 설정------------------------------------------------------------------------
file_name = '2024-04-09-16-43-56'
b = bagreader(file_name+'.bag')
x_min = 60239.34
x_max = 60240.36
#-------------------------------------------------------------------------------------------------------------------------------------------
imu_msg = b.message_by_topic(topic='/bldc_test')# 토픽.csv생성
pwm_msg = b.message_by_topic(topic='/PWM')

df_imu = pd.read_csv(file_name+'/PWM.csv')
df_PWM = pd.read_csv(file_name+'/bldc_test.csv')
df_imu['Time'] = pd.to_datetime(df_imu['Time'], unit='s')
df_PWM['Time'] = pd.to_datetime(df_PWM['Time'], unit='s')
imu_list1 = [x for x in df_imu['data_0']]# Roll
imu_list2 = [x for x in df_imu['data_1']]# Pitch
imu_list3 = [x for x in df_imu['data_2']]# Yaw
imu_list4 = [x for x in df_imu['data_3']]# w_x
#imu_list5 = [x for x in df_imu['data_4']]# w_y
#imu_list6 = [x for x in df_imu['data_5']]# w_z
numbers_imu = [list(imu_data) for imu_data in zip(imu_list1)]#, imu_list2)]#, imu_list3, imu_list4, imu_list5, imu_list6)]
pwm_list1 = [x for x in df_PWM['data_0']]
pwm_list2 = [x for x in df_PWM['data_1']]
pwm_list3 = [x for x in df_PWM['data_2']]
pwm_list4 = [x for x in df_PWM['data_3']]
numbers_pwm = [list(pwm_data) for pwm_data in zip(pwm_list1)]#, pwm_list2, pwm_list3, pwm_list4)]

time_values_imu = df_imu['Time'].dt.strftime('%H:%M:%S.%f').str.split(':').apply(lambda x: int(x[0]) * 3600 + int(x[1]) * 60 + float(x[2]))
time_values_pwm = df_PWM['Time'].dt.strftime('%H:%M:%S.%f').str.split(':').apply(lambda x: int(x[0]) * 3600 + int(x[1]) * 60 + float(x[2]))
'''
selected_time_values_imu = time_values_imu[(time_values_imu >= x_min) & (time_values_imu <= x_max)]
selected_numbers_imu = [[array[i] for i in selected_imu_index] for array in numbers_imu if x_min <= time_values_imu[numbers_imu.index(array)] <= x_max]
selected_time_values_imu = selected_time_values_imu - x_min
'''
selected_time_values_imu = time_values_imu[(time_values_imu >= x_min) & (time_values_imu <= x_max)]
selected_numbers_imu = [numbers_imu[i] for i in range(len(numbers_imu)) if x_min <= time_values_imu[i] <= x_max]
selected_time_values_imu = selected_time_values_imu - x_min

selected_time_values_pwm = time_values_pwm[(time_values_pwm >= x_min) & (time_values_pwm <= x_max)]
selected_numbers_pwm = [numbers_pwm[i] for i in range(len(numbers_pwm)) if x_min <= time_values_pwm[i] <= x_max]
selected_time_values_pwm = selected_time_values_pwm - x_min

for i in range(len(selected_numbers_pwm)):
    #center_value = max(selected_numbers_pwm[i]) / 2 + min(selected_numbers_pwm[i]) / 2
    #selected_numbers_pwm[i] = [x - center_value for x in selected_numbers_pwm[i]]
    selected_numbers_pwm[i] = [x for x in selected_numbers_pwm[i]]#/20

#roll_data = [[pwm[0] + pwm[1] - pwm[2] - pwm[3]] for pwm in selected_numbers_pwm]
#pitch_data = [[pwm[0] - pwm[1] - pwm[2] + pwm[3]] for pwm in selected_numbers_pwm]

plt.figure(1)
plt.plot(selected_time_values_imu, selected_numbers_imu, label='bldc Data')
plt.plot(selected_time_values_pwm, selected_numbers_pwm, label='PWM Data')
#plt.plot(selected_time_values_pwm, roll_data, label='Roll Data')
#plt.plot(selected_time_values_pwm, pitch_data, label='Pitch Data')
plt.xlabel('Time (seconds)')
plt.ylabel('PWM, bldc')#('imu to Roll, Pitch')
plt.title('interval(PWM, bldc)')#('PWM and Selected IMU Data Comparison (X axis range: {} - {})'.format(x_min, x_max))
plt.legend()
#plt.figure(2)
#plt.plot(selected_time_values_pwm, selected_numbers_pwm, label='PWM Data')
#plt.xlabel('Time (seconds)')
#plt.ylabel('Scaled Value')
#plt.title('PWM and Selected IMU Data Comparison (X axis range: {} - {})'.format(x_min, x_max))
#plt.legend()
plt.show()
