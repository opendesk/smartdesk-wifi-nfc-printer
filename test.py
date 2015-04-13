import requests
import time
import urllib

def main():
    current_data = False
    while(1):
        try:
            r = requests.get('http://www.smartdesk.cc/new_value')
            if r.json()['has'] == u'yes':
                r = requests.get('http://www.smartdesk.cc/actions')
                json_data = r.json()
                for k,v in json_data.items():
                    encoded_url = urllib.quote_plus(''.join(v.split(',')[0]
                        ).split(':')[-1].replace('"',''))

                    url = 'http://localhost/mailbox/{0}'.format(encoded_url)
                    requests.post(url)

        except (requests.exceptions.ConnectionError, ValueError):
            pass

        time.sleep(2)

if __name__ == "__main__":
    time.sleep(5)
    main()
