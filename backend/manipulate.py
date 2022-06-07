import database
from database import DBSession

db = DBSession()
res = db.query(database.Benchmark).get('758e92ae-2990-4597-8d7b-2326824f901b')
print(res.data)
res.data = [
            {'pwm': 1500, 'current_rpm': 1000, 'current_amperes': 7.47, 'current_voltage': 23.89, 'tensometer1': 642,
             'tensometer2': 0, 'tensometer3': 0, 'running': True},
            {'pwm': 1550, 'current_rpm': 1000, 'current_amperes': 9.48, 'current_voltage': 23.89, 'tensometer1': 749,
             'tensometer2': 0, 'tensometer3': 0, 'running': True},
            {'pwm': 1600, 'current_rpm': 1000, 'current_amperes': 11.67, 'current_voltage': 23.89, 'tensometer1': 866,
             'tensometer2': 0, 'tensometer3': 0, 'running': True},
            {'pwm': 1650, 'current_rpm': 1000, 'current_amperes': 14.38, 'current_voltage': 23.89, 'tensometer1': 990,
             'tensometer2': 0, 'tensometer3': 0, 'running': True},
            {'pwm': 1700, 'current_rpm': 1000, 'current_amperes': 17.31, 'current_voltage': 23.89, 'tensometer1': 1100,
             'tensometer2': 0, 'tensometer3': 0, 'running': True},
            {'pwm': 1750, 'current_rpm': 1000, 'current_amperes': 19.87, 'current_voltage': 23.89, 'tensometer1': 1181,
             'tensometer2': 0, 'tensometer3': 0, 'running': True},
            {'pwm': 1800, 'current_rpm': 1000, 'current_amperes': 22.97, 'current_voltage': 23.89, 'tensometer1': 1284,
             'tensometer2': 0, 'tensometer3': 0, 'running': True},
            {'pwm': 1850, 'current_rpm': 1000, 'current_amperes': 26.09, 'current_voltage': 23.89, 'tensometer1': 1380,
             'tensometer2': 0, 'tensometer3': 0, 'running': True},
            {'pwm': 1900, 'current_rpm': 1000, 'current_amperes': 29.32, 'current_voltage': 23.89, 'tensometer1': 1476,
             'tensometer2': 0, 'tensometer3': 0, 'running': True},
            {'pwm': 1950, 'current_rpm': 1000, 'current_amperes': 32.56, 'current_voltage': 23.89, 'tensometer1': 1511,
             'tensometer2': 0, 'tensometer3': 0, 'running': True},
            {'pwm': 2000, 'current_rpm': 1000, 'current_amperes': 40.48, 'current_voltage': 23.89, 'tensometer1': 1767,
             'tensometer2': 0, 'tensometer3': 0, 'running': True}]

for item in res.data:
    item['current_amperes'] += 0.26
    item['current_voltage'] += 0.13
    item['tensometer1'] += item['tensometer1'] * 0.08

db.commit()
