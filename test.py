import requests
import time

def main():
    current_data = False
    while(1):
        #r = requests.get('http://www.smartdesk.cc/new_value')
        if 1:#r.json()['new_value'] == u'yes':
            r = requests.get('http://www.smartdesk.cc/actions')
            try:
                json_data = r.json()
                if current_data != json_data:
                    current_data = json_data
                    for k,v in json_data.items():
                        url = 'http://localhost/mailbox/{0}'.format(''.join(v.split(',')[0]).split(':')[-1].replace('"','').replace(' ','%20'))
                        requests.post(url)
            except ValueError:
                pass
        time.sleep(2)

if __name__ == "__main__":
    main()
