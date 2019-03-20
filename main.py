# from network import WLAN
# wlan = WLAN(mode=WLAN.STA)
# wlan.connect(ssid='eduroam', auth=(WLAN.WPA2_ENT, 'username', 'password'), identity='identity')


def do_connect():
    import network
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print('connecting to network...')
        wlan.connect(ssid='IITD_WIFI', auth=(network.WLAN.WPA2_ENT, 'username', 'password'), identity='identity')
        wlan.connect('essid', 'password')
        while not wlan.isconnected():
            pass
    print('network config:', wlan.ifconfig())

do_connect()