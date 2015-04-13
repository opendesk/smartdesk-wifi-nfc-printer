import requests
import time
import urllib

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
                        encoded_url = urllib.quote_plus(''.join(v.split(',')[0]
                            ).split(':')[-1].replace('"',''))

                        url = 'http://localhost/mailbox/{0}'.format(encoded_url)
                        requests.post(url)

            except (requests.exceptions.ConnectionError, ValueError):
                pass
        time.sleep(2)

if __name__ == "__main__":
    main()
