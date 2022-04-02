import random

NUMBER_OF_GENERATING_LINES = 5

towns = ["Moscow", "Samara", "Kazan", "Omsk", "Ufa"]
transports = ["Bus", "Train", "Plane"]
bus_brands = ["Mersedes-Benz", "Toyota", "BMW"]
train_types = ["passenger", "cargo", "high-speed"]
plane_types = ["bomber", "fighter", "scout"]
roads = dict()
roads[("Moscow", "Samara")] = 1050
roads[("Moscow", "Kazan")] = 815
roads[("Moscow", "Omsk")] = 2700
roads[("Moscow", "Ufa")] = 1350
roads[("Samara", "Kazan")] = 370
roads[("Samara", "Omsk")] = 1790
roads[("Samara", "Ufa")] = 462
roads[("Kazan", "Omsk")] = 1890
roads[("Kazan", "Ufa")] = 550
roads[("Omsk", "Ufa")] = 1300

with open("input.txt", 'w') as f :
    for i in range(NUMBER_OF_GENERATING_LINES) :
        tr = random.choice(transports)
        if tr == "Bus" :
            speed = (1 + random.randint(0, 20) / 100) * 50
            type = random.choice(bus_brands)
        elif tr == "Train" :
            speed = speed = (1 + random.randint(0, 20) / 100) * 90
            type = random.choice(train_types)
        else :
            speed = speed = (1 + random.randint(0, 20) / 100) * 700
            type = random.choice(plane_types)
        t1 = random.choice(towns)
        t2 = random.choice(towns)
        while t1 == t2 :
           t2 = random.choice(towns)
        print(tr, type, int(speed), t1, t2, end = ' ', file = f)
        if (t1, t2) in roads.keys() : 
            print(roads[(t1, t2)], file = f)
        else :
            print(roads[(t2, t1)], file = f)
            