import sys

Fcy = 60e6 #osc:60Mhz
Z_pulse_per_round = 500

def main():

    # Z rpm table generator
    z_rpm_list = list(range(100,3700,100))
    z_rpm_pulse_prescale_1 = []
    z_rpm_dict = {}
    for z_rpm in z_rpm_list:
        z_pulse_t = 60/z_rpm/Z_pulse_per_round
        z_pulse_period = Fcy*z_pulse_t
        z_rpm_pulse_prescale_1.append(z_pulse_period) 
        z_rpm_dict[str(z_rpm)+'rpm'] = [z_rpm,'{:4.1f}us'.format(z_pulse_t*1e6),int(z_pulse_period)]

    for idx, _z in enumerate(z_rpm_dict):
        print(idx,_z,z_rpm_dict[_z])
    
    print('enum Zrpm {')
    for idx, _z in enumerate(z_rpm_dict):
        log = '     z_%s = %d,'%(_z,idx)
        print(log)
    print('     z_rpm_max,')
    print('};')

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

if __name__ == '__main__':
    main()