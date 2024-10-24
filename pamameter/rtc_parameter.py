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
    z_rpm_list = list(range(50,3000,5))
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
    n = 1
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
        n+=1
    print('n===:',n)
    
    for idx, k in enumerate(cn_dict):
        print(idx,k,cn_dict[k])

    #    z_pulse_t = 60/z_rpm/z_spr
    #     z_pulse_period = Fcy*z_pulse_t 

def pulse_calculation(max_rpm,spr):
    iFcy = 60e6
    iMaxSpd = max_rpm
    iMaxSpd_period_count = int( (60/iMaxSpd/spr)*iFcy)
    log = 'max prm:%d, period_cnt:%d'%(iMaxSpd,iMaxSpd_period_count)
    print(log)

    f_accel = iMaxSpd/14
    f_accel = 5 if f_accel < 5 else f_accel

    c0_f_accel_pulse_count = int((60/f_accel/spr)*iFcy)
    print('pulse_cnt c0:',c0_f_accel_pulse_count)
    cn=[c0_f_accel_pulse_count]
    for n in range(1,400):
        cn_f_accel = cn[0]*( math.sqrt(n+1)-math.sqrt(n))
        cn.append(int(cn_f_accel))
        if cn_f_accel< iMaxSpd_period_count:
            cn[n]=iMaxSpd_period_count
            break
    log = 'cn:%s,---len:%d'%(cn,len(cn))
    print(log)
    
    cm=[c0_f_accel_pulse_count]
    for m in range(1,400):
        cm_f_accel = cm[m-1] - ( 2* cm[m-1]/(4*m+1))
        if m == 1:
            cm_f_accel = cm_f_accel/1.448528938
        cm.append(int(cm_f_accel))
        if cm_f_accel< iMaxSpd_period_count:
            cm[m]=iMaxSpd_period_count
            break
    log = 'cm:%s,---len:%d'%(cm,len(cm))
    print(log)

    # rpm_cn_conv = []
    # for idx,k in enumerate(cn):
    #     rpm_cn_conv.append( 60*iFcy/spr/k)
    cn_rpm = []
    for idx, cn in enumerate(cn):
        cn_rpm.append(float('{:4.1f}'.format(60*iFcy/spr/cn)))
    cm_rpm = []
    for idx, cm in enumerate(cm):
        cm_rpm.append(float('{:4.1f}'.format(60*iFcy/spr/cm)))
    print('cn_rpm:',cn_rpm)
    print('cm_rpm:',cm_rpm)
    # print(rpm_cn_conv)

    cn_prm_s = []
    for n in range(1,len(cn_rpm)):
        cn_prm_s.append(float('{:4.2f}'.format(cn_rpm[n]-cn_rpm[n-1])))
    print('cn_prm_s:',cn_prm_s)
    
    cm_prm_s = []
    for n in range(1,len(cm_rpm)):
        cm_prm_s.append(float('{:4.2f}'.format(cm_rpm[n]-cm_rpm[n-1])))
    print('cm_prm_s:',cm_prm_s)

    # rpm_s = []
    # for r in range(1,len(rpm_cn_conv)):
    #     rpm_s.append(rpm_cn_conv[r]-rpm_cn_conv[r-1])

    # for _rpm in rpm_s:
    #     print('{:4.3f}rpm_s'.format(_rpm))
    # print('len(rpm_s):',len(rpm_s))

#   start_rpm = 100, end_rpm = 1000, 1600
def pulse_calculation2(start_rpm,end_rpm,spr):  
    iFcy = 60e6
    end_rpm_period_cnt = int( (60/end_rpm/spr)*iFcy)
    start_rpm_period_cnt = int( (60/start_rpm/spr)*iFcy)
    log = 'end rpm:%d, period_cnt:%d'%(end_rpm,end_rpm_period_cnt)
    print(log)
    log = 'start rpm:%d, period_cnt:%d'%(start_rpm,start_rpm_period_cnt)
    print(log)

    f_accel = (end_rpm-start_rpm)/14
    f_accel = 5 if f_accel < 5 else f_accel
    c0_f_accel_pulse_count = int((60/(start_rpm+(f_accel)/32)/spr)*iFcy)
    cn=[c0_f_accel_pulse_count]
    for n in range(1,400):
        cn_f_accel = cn[0]*( math.sqrt(n+1)-math.sqrt(n))
        cn.append(int(cn_f_accel))
        if cn_f_accel< end_rpm_period_cnt:
            cn[n]=end_rpm_period_cnt
            break
    log = 'cn:%s,---len:%d'%(cn,len(cn))
    print(log)

    cm=[c0_f_accel_pulse_count]
    for m in range(1,400):
        cm_f_accel = cm[m-1] - ( 2* cm[m-1]/(4*m+1))
        if m == 1:
            cm_f_accel = cm_f_accel/1.448528938
        cm.append(int(cm_f_accel))
        if cm_f_accel< end_rpm_period_cnt:
            cm[m]=end_rpm_period_cnt
            break
    log = 'cm:%s,---len:%d'%(cm,len(cm))
    print(log)

def pulse_calculation2_1(start_rpm,end_rpm,spr):  
    iFcy = 60e6
    end_rpm_period_cnt = int( (60/end_rpm/spr)*iFcy)
    start_rpm_period_cnt = int( (60/start_rpm/spr)*iFcy)
    log = 'end rpm:%d, period_cnt:%d'%(end_rpm,end_rpm_period_cnt)
    print(log)
    log = 'start rpm:%d, period_cnt:%d'%(start_rpm,start_rpm_period_cnt)
    print(log)
    c0_f_accel_pulse_count = int((60/(start_rpm)/spr)*iFcy)
    c0_f_acc_pulse_count = int((60/(end_rpm_period_cnt)/spr)*iFcy)
    cm=[c0_f_accel_pulse_count]
    for m in range(1,400):
        #cm_f_accel = cm[m-1] - ( 2* cm[m-1]/(4*m+1))
        
        # if m == 1:
        #     cm_f_accel = cm_f_accel/1.448528938

        cm_f_accel = cm[m-1]-c0_f_acc_pulse_count/8
        cm.append(int(cm_f_accel))
        if cm_f_accel< end_rpm_period_cnt:
            cm[m]=end_rpm_period_cnt
            break
    log = 'cm:%s,---len:%d'%(cm,len(cm))
    print(log)

#   start_rpm = 100, end_rpm = 1000, 1600
def pulse_calculation3(start_rpm,end_rpm,spr):  
    iFcy = 60e6
    end_rpm_period_cnt = int( (60/end_rpm/spr)*iFcy)
    start_rpm_period_cnt = int( (60/start_rpm/spr)*iFcy)
    log = 'end rpm:%d, period_cnt:%d'%(end_rpm,end_rpm_period_cnt)
    print(log)
    log = 'start rpm:%d, period_cnt:%d'%(start_rpm,start_rpm_period_cnt)
    print(log)

    f_accel = (end_rpm-start_rpm)/14
    log = 'f_accel:%d'%(f_accel)
    print(log)

    #f_accel = 5 if f_accel < 5 else f_accel
    log = 'f_accel:%d'%(f_accel)
    print(log)

    c0_f_accel_pulse_count = int((60/(start_rpm)/spr)*iFcy)
    c0_eff_accel_pulse_count =  int((60/(0+(abs(f_accel)*32))/spr)*iFcy)
    # log = 'f_eff_accel:%d'%(c0_eff_accel_pulse_count)
    # print(log)
    # cn=[c0_f_accel_pulse_count]
    # for n in range(1,400):
    #     cn_f_accel = cn[0]*( math.sqrt(n+1)-math.sqrt(n))
    #     print(n,cn_f_accel)
    #     cn_f_accel += c0_eff_accel_pulse_count
    #     print(n,cn_f_accel)
    #     cn.append(int(cn_f_accel))
    #     if cn_f_accel > end_rpm_period_cnt:
    #         cn[n]=end_rpm_period_cnt
    #         break
    # log = 'cn:%s,---len:%d'%(cn,len(cn))
    # print(log)

    cm=[c0_f_accel_pulse_count]
    for m in range(1,400):
        cm_f_accel = cm[m-1] - ( 2* cm[m-1]/(4*m+1))
        cm_f_accel+=cm[0]
        if m == 1:
            cm_f_accel = cm_f_accel/1.448528938
        cm.append(int(cm_f_accel))
        if cm_f_accel> end_rpm_period_cnt:
            cm[m]=end_rpm_period_cnt
            break
    log = 'cm:%s,---len:%d'%(cm,len(cm))
    print(log)

def pulse_calculation2_2(start_rpm,end_rpm,spr):  
    iFcy = 60e6

    #f_accel = 5 if f_accel < 5 else f_accel
    s_rpm = 5 if start_rpm < 5 else start_rpm
    e_rpm = 5 if end_rpm <5 else end_rpm

    end_rpm_period_cnt = int( (60/e_rpm/spr)*iFcy)
    start_rpm_period_cnt = int( (60/s_rpm/spr)*iFcy)
    log = 'e_rpm:%d p_cnt:%d, s_rpm:%d p_cnt:%d'\
        %(e_rpm,end_rpm_period_cnt,s_rpm,start_rpm_period_cnt)
    print(log)

    c0_f_accel_pulse_count = int((60/(s_rpm)/spr)*iFcy)
    c0_f_acc_pulse_count = int((60/(end_rpm_period_cnt)/spr)*iFcy)
    cm=[c0_f_accel_pulse_count]
    for m in range(1,400):
        cm_f_accel = cm[m-1]-c0_f_acc_pulse_count/8
        cm.append(int(cm_f_accel))

        if e_rpm > s_rpm:
            if cm_f_accel< end_rpm_period_cnt:
                cm[m]=end_rpm_period_cnt
                break
        else:
            if cm_f_accel> end_rpm_period_cnt:
                cm[m]=end_rpm_period_cnt
                break
    log = 'cm:%s,---len:%d'%(cm,len(cm))
    print(log)


def pulse_calculation4(start_rpm,end_rpm,spr):  
    iFcy = 60e6
    s_rpm = 5 if start_rpm < 5 else start_rpm
    e_rpm = 5 if end_rpm <5 else end_rpm

    if s_rpm < e_rpm:
        print('------------------1')
        e_rpm_percnt = int( (60/e_rpm/spr)*iFcy)
        s_rpm_percnt = int( (60/s_rpm/spr)*iFcy)
        log = 'end-rpm:%d, period_cnt:%d'%(e_rpm,e_rpm_percnt)
        print(log)
        log = 'start-rpm:%d, period_cnt:%d'%(s_rpm,s_rpm_percnt)
        print(log)

        c0_accel = int((60/(s_rpm)/spr)*iFcy)
        c0_inc = int((60/(e_rpm_percnt)/spr)*iFcy)
        cm=[c0_accel]
        for m in range(1,16000):
            cm_f_accel = cm[m-1]- (2* cm[m-1]/(4*m+1))
            cm.append(int(cm_f_accel))
            if cm_f_accel < e_rpm_percnt:
                cm[m] = e_rpm_percnt
                break
        log = 'cm:%s,---len:%d'%(cm,len(cm))
        print(log)

    else:
        print('------------------2')

        e_rpm_percnt = int( (60/e_rpm/spr)*iFcy)
        s_rpm_percnt = int( (60/s_rpm/spr)*iFcy)
        log = 'end-rpm:%d, period_cnt:%d'%(e_rpm,e_rpm_percnt)
        print(log)
        log = 'start-rpm:%d, period_cnt:%d'%(s_rpm,s_rpm_percnt)
        print(log)

        f_accel = (e_rpm-s_rpm)/14
        log = 'f_accel:%d'%(f_accel)
        print(log)
        c0_deaccel = int((60/(s_rpm)/spr)*iFcy)
        cm=[c0_deaccel]
        for m in range(1,16000):
            #cm_f_accel = cm[m-1] - ( 2* cm[m-1]/(4*m+1))
            cm_f_accel = cm[m-1] - ( 2* cm[m-1]/(4*m+1))
            cm_f_accel+=cm[0]
            # if m == 1:
            #     cm_f_accel = cm_f_accel/1.448528938
            cm.append(int(cm_f_accel))
            if cm_f_accel> e_rpm_percnt:
                cm[m]=e_rpm_percnt
                break
        log = 'cm:%s,---len:%d'%(cm,len(cm))
        print(log)

if __name__ == '__main__':
    main()
    #pulse_calculation(3000,1600)
    #pulse_calculation2(200,600,1600)
    #pulse_calculation2_1(100,3000,1600)
    #pulse_calculation3(3000,10,1600)
    #pulse_calculation3(500,100,1600)

    #pulse_calculation4(5,600,1600) 