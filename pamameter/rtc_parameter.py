import sys

Fcy = 60e6 #osc:60Mhz
z_spr = 1600
ALPHA = 2*3.14159/z_spr
A_T_x100 =ALPHA*Fcy*100
T1_FREQ_148 = (Fcy*0.676)/100
A_SQ =ALPHA*2*1e10
A_x20000 =(ALPHA*20000)
rpm_to_rad = 1/9.55
import math

def main():

    # Z rpm table generator
    z_rpm_list = list(range(5,25,5))
    print('z_rpm_list:',z_rpm_list)
    z_rpm_pulse_prescale_1 = []
    z_rpm_dict = {}
    for z_rpm in z_rpm_list:
        z_pulse_t = 60/z_rpm/z_spr
        z_pulse_period = Fcy*z_pulse_t
        z_rpm_pulse_prescale_1.append(z_pulse_period) 
        z_rpm_dict[str(z_rpm)+'rpm'] = [z_rpm,'{:4.1f}us'.format(z_pulse_t*1e6),int(z_pulse_period)]

    for idx, _z in enumerate(z_rpm_dict):
        print('000',idx,_z,z_rpm_dict[_z])
    
    # print('enum Zrpm {')
    # for idx, _z in enumerate(z_rpm_dict):
    #     log = '     z_%s = %d,'%(_z,idx)
    #     print(log)
    # print('     z_rpm_max,')
    # print('};')

    print('OC_1_2_CascadeDef_t OC_1_2_cascade_def[] FAR={')
    for idx, _z in enumerate(z_rpm_dict):
        t_period = z_rpm_dict[_z][2]
        t_dutyon = int(t_period/2)
        a0_period = t_period & 0xffff
        a1_period = (t_period >> 16) & 0xffff
        a0_dutyon = t_dutyon & 0xffff
        a1_dutyon = (t_dutyon >> 16) & 0xffff
        log = '{%d,{0x%s,0x%s,0x%s,0x%s}},'%(z_rpm_dict[_z][0],
                                           '{:04x}'.format(a1_period),
                                           '{:04x}'.format(a0_period),
                                           '{:04x}'.format(a1_dutyon),
                                           '{:04x}'.format(a0_dutyon))
        print(log)
    print('};')

    #---------------------avr446
    # steps = 5000
    # speeds = 2000 #200rpm
    # accel = 8000 
    # decel = 8000 
    # min_delay = A_T_x100 / speeds
    # print("min_delay(200rpm) actual(191rpm):",int(min_delay))

    # step_delay = T1_FREQ_148 * math.sqrt(A_SQ/accel)/100
    # print("step_delay(200rpm):",int(step_delay))

    # #max_s_lim = (long)speeds*speeds/(long)(((long)A_x20000*accel)/100);/
    # max_s_lim = speeds*speeds/(A_x20000*accel/100)
    # print("max_s_lim(steps):",int(max_s_lim))

    # accel_lim = steps*decel/(accel+decel)#((long)steps*decel) / (accel+decel);
    # print("accel_lim(steps):",int(accel_lim))

    start_speed = 4
    max_speed = 200 # assume max spd = 200rpm
    max_spd_rad_per_sec = max_speed * rpm_to_rad
    min_delay = A_T_x100 /max_spd_rad_per_sec / 100
    print('min_delay (src:%drpm)=%d count'%(max_speed,min_delay))

    accel_rpm = max_speed * 3
    accel = accel_rpm * rpm_to_rad
    step_delay = T1_FREQ_148 * math.sqrt(A_SQ/accel)/100
    print('step_delay (accel:%drpm)=%d count'%(accel_rpm,step_delay))

    v=(max_speed* rpm_to_rad)
    u=(start_speed* rpm_to_rad)
    acc = (v*v-u*u)/(2*ALPHA)
    print('acc =%d '%(acc))

    #c_0=z_rpm_dict['5rpm'][2]
    maxspd=20
    print('---maxspd:',maxspd)
    if maxspd >=14:
        z_rpm = maxspd/14
    else:
        z_rpm = maxspd
    if z_rpm < 5:
        z_rpm = 5
    # if z_rpm < 12.2:
    #     z_rpm = 12.2
    z_pulse_t = 60/z_rpm/z_spr
    z_pulse_period = Fcy*z_pulse_t

    z_max_spd_period = 60/maxspd/z_spr*Fcy
    print('z_max_spd_period:',z_max_spd_period)
    c_0 = z_pulse_period
    cn_dict={'c0':[c_0,c_0,'{:4.1f}us'.format((c_0/Fcy)*1e6),str(60/(c_0/Fcy)/z_spr)+'rpm']}
    #print(cn_dict)
    for i in range(1,500):
        cn_va1 = int(cn_dict['c0'][0]*(math.sqrt(i+1)-math.sqrt(i)))
        cn_va2 = (cn_dict['c%d'%(i-1)][1]-int((2* cn_dict['c%d'%(i-1)][1])/(4*i+1)))
        if i == 1:
            cn_va2 /= 1.46
        cn_dict['c%d'%(i)] = [cn_va1,int(cn_va2),'{:4.1f}us'.format((cn_va1/Fcy)*1e6),
        '{:4.1f}prm'.format((60/(cn_va1/Fcy)/z_spr)),
        '{:4.1f}prm2'.format((60/(cn_va2/Fcy)/z_spr)),
        ]
        print('cn_va2---:',cn_va2)

        if (cn_va2) < z_max_spd_period:
           break
    
    for idx, k in enumerate(cn_dict):
        print(idx,k,cn_dict[k])

    #    z_pulse_t = 60/z_rpm/z_spr
    #     z_pulse_period = Fcy*z_pulse_t 



if __name__ == '__main__':
    main()