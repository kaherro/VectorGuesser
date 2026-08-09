CREATE TABLE IF NOT EXISTS cities (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    latitude REAL NOT NULL,
    longitude REAL NOT NULL,
    country TEXT NOT NULL
);

INSERT OR IGNORE INTO cities (name, latitude, longitude, country) VALUES
    -- USA
    ('New York', 40.7128, -74.0060, 'USA'),
    ('Los Angeles', 34.0522, -118.2437, 'USA'),
    ('Chicago', 41.8781, -87.6298, 'USA'),
    ('Houston', 29.7604, -95.3698, 'USA'),
    ('Miami', 25.7617, -80.1918, 'USA'),
    ('Seattle', 47.6062, -122.3321, 'USA'),
    ('San Francisco', 37.7749, -122.4194, 'USA'),
    ('Boston', 42.3601, -71.0589, 'USA'),
    ('Washington', 38.9072, -77.0369, 'USA'),
    ('Philadelphia', 39.9526, -75.1652, 'USA'),
    ('Atlanta', 33.7490, -84.3880, 'USA'),
    ('Denver', 39.7392, -104.9903, 'USA'),
    ('Dallas', 32.7767, -96.7970, 'USA'),
    ('Phoenix', 33.4484, -112.0740, 'USA'),

    -- Russia
    ('Moscow', 55.7558, 37.6173, 'Russia'),
    ('Saint Petersburg', 59.9343, 30.3351, 'Russia'),
    ('Novosibirsk', 55.0084, 82.9357, 'Russia'),
    ('Yekaterinburg', 56.8389, 60.6057, 'Russia'),

    -- UK / Ireland
    ('London', 51.5074, -0.1278, 'UK'),
    ('Birmingham', 52.4862, -1.8904, 'UK'),
    ('Manchester', 53.4808, -2.2426, 'UK'),
    ('Glasgow', 55.8642, -4.2518, 'UK'),
    ('Edinburgh', 55.9533, -3.1883, 'UK'),
    ('Dublin', 53.3498, -6.2603, 'Ireland'),

    -- France
    ('Paris', 48.8566, 2.3522, 'France'),
    ('Marseille', 43.2965, 5.3698, 'France'),
    ('Lyon', 45.7640, 4.8357, 'France'),
    ('Toulouse', 43.6047, 1.4442, 'France'),

    -- Germany / Austria / Switzerland
    ('Berlin', 52.5200, 13.4050, 'Germany'),
    ('Hamburg', 53.5511, 9.9937, 'Germany'),
    ('Munich', 48.1351, 11.5820, 'Germany'),
    ('Cologne', 50.9375, 6.9603, 'Germany'),
    ('Vienna', 48.2082, 16.3738, 'Austria'),
    ('Zurich', 47.3769, 8.5417, 'Switzerland'),
    ('Geneva', 46.2044, 6.1432, 'Switzerland'),

    -- Italy / Spain / Portugal
    ('Rome', 41.9028, 12.4964, 'Italy'),
    ('Milan', 45.4642, 9.1900, 'Italy'),
    ('Naples', 40.8518, 14.2681, 'Italy'),
    ('Madrid', 40.4168, -3.7038, 'Spain'),
    ('Barcelona', 41.3851, 2.1734, 'Spain'),
    ('Valencia', 39.4699, -0.3763, 'Spain'),
    ('Lisbon', 38.7223, -9.1393, 'Portugal'),
    ('Porto', 41.1579, -8.6291, 'Portugal'),

    -- Benelux
    ('Amsterdam', 52.3676, 4.9041, 'Netherlands'),
    ('Rotterdam', 51.9244, 4.4777, 'Netherlands'),
    ('Brussels', 50.8503, 4.3517, 'Belgium'),
    ('Antwerp', 51.2194, 4.4025, 'Belgium'),

    -- Northern / Eastern Europe
    ('Stockholm', 59.3293, 18.0686, 'Sweden'),
    ('Gothenburg', 57.7089, 11.9746, 'Sweden'),
    ('Oslo', 59.9139, 10.7522, 'Norway'),
    ('Copenhagen', 55.6761, 12.5683, 'Denmark'),
    ('Helsinki', 60.1699, 24.9384, 'Finland'),
    ('Warsaw', 52.2297, 21.0122, 'Poland'),
    ('Krakow', 50.0647, 19.9450, 'Poland'),
    ('Prague', 50.0755, 14.4378, 'Czech Republic'),
    ('Budapest', 47.4979, 19.0402, 'Hungary'),
    ('Bucharest', 44.4268, 26.1025, 'Romania'),
    ('Sofia', 42.6977, 23.3219, 'Bulgaria'),
    ('Athens', 37.9838, 23.7275, 'Greece'),
    ('Kyiv', 50.4501, 30.5234, 'Ukraine'),

    -- Asia — East
    ('Tokyo', 35.6762, 139.6503, 'Japan'),
    ('Osaka', 34.6937, 135.5023, 'Japan'),
    ('Yokohama', 35.4437, 139.6380, 'Japan'),
    ('Beijing', 39.9042, 116.4074, 'China'),
    ('Shanghai', 31.2304, 121.4737, 'China'),
    ('Guangzhou', 23.1291, 113.2644, 'China'),
    ('Shenzhen', 22.5431, 114.0579, 'China'),
    ('Chengdu', 30.5728, 104.0668, 'China'),
    ('Wuhan', 30.5928, 114.3055, 'China'),
    ('Xian', 34.3416, 108.9398, 'China'),
    ('Seoul', 37.5665, 126.9780, 'South Korea'),
    ('Busan', 35.1796, 129.0756, 'South Korea'),

    -- Asia — South / Southeast
    ('Mumbai', 19.0760, 72.8777, 'India'),
    ('Delhi', 28.7041, 77.1025, 'India'),
    ('Bangalore', 12.9716, 77.5946, 'India'),
    ('Kolkata', 22.5726, 88.3639, 'India'),
    ('Chennai', 13.0827, 80.2707, 'India'),
    ('Hyderabad', 17.3850, 78.4867, 'India'),
    ('Jakarta', -6.2088, 106.8456, 'Indonesia'),
    ('Surabaya', -7.2575, 112.7521, 'Indonesia'),
    ('Manila', 14.5995, 120.9842, 'Philippines'),
    ('Bangkok', 13.7563, 100.5018, 'Thailand'),
    ('Hanoi', 21.0285, 105.8542, 'Vietnam'),
    ('Ho Chi Minh City', 10.8231, 106.6297, 'Vietnam'),
    ('Singapore', 1.3521, 103.8198, 'Singapore'),
    ('Kuala Lumpur', 3.1390, 101.6869, 'Malaysia'),
    ('Karachi', 24.8607, 67.0011, 'Pakistan'),
    ('Lahore', 31.5497, 74.3436, 'Pakistan'),
    ('Dhaka', 23.8103, 90.4125, 'Bangladesh'),

    -- Middle East / Central Asia
    ('Istanbul', 41.0082, 28.9784, 'Turkey'),
    ('Ankara', 39.9334, 32.8597, 'Turkey'),
    ('Tehran', 35.6892, 51.3890, 'Iran'),
    ('Riyadh', 24.7136, 46.6753, 'Saudi Arabia'),
    ('Jeddah', 21.4858, 39.1925, 'Saudi Arabia'),
    ('Dubai', 25.2048, 55.2708, 'UAE'),
    ('Abu Dhabi', 24.4539, 54.3773, 'UAE'),
    ('Tel Aviv', 32.0853, 34.7818, 'Israel'),
    ('Jerusalem', 31.7683, 35.2137, 'Israel'),
    ('Baghdad', 33.3152, 44.3661, 'Iraq'),
    ('Almaty', 43.2220, 76.8512, 'Kazakhstan'),
    ('Astana', 51.1694, 71.4491, 'Kazakhstan'),
    ('Tashkent', 41.2995, 69.2401, 'Uzbekistan'),
    ('Yerevan', 40.1792, 44.4991, 'Armenia'),
    ('Tbilisi', 41.7151, 44.8271, 'Georgia'),
    ('Baku', 40.4093, 49.8671, 'Azerbaijan'),
    ('Dushanbe', 38.5598, 68.7870, 'Tajikistan'),

    -- Canada / Mexico
    ('Toronto', 43.6511, -79.3470, 'Canada'),
    ('Vancouver', 49.2827, -123.1207, 'Canada'),
    ('Montreal', 45.5017, -73.5673, 'Canada'),
    ('Mexico City', 19.4326, -99.1332, 'Mexico'),
    ('Guadalajara', 20.6597, -103.3496, 'Mexico'),

    -- South America
    ('Sao Paulo', -23.5505, -46.6333, 'Brazil'),
    ('Rio de Janeiro', -22.9068, -43.1729, 'Brazil'),
    ('Brasilia', -15.8267, -47.9218, 'Brazil'),
    ('Buenos Aires', -34.6037, -58.3816, 'Argentina'),
    ('Santiago', -33.4489, -70.6693, 'Chile'),
    ('Lima', -12.0464, -77.0428, 'Peru'),
    ('Bogota', 4.7110, -74.0721, 'Colombia'),
    ('Caracas', 10.4806, -66.9036, 'Venezuela'),
    ('Quito', -0.1807, -78.4678, 'Ecuador'),
    ('Montevideo', -34.9011, -56.1645, 'Uruguay'),

    -- Africa
    ('Cairo', 30.0444, 31.2357, 'Egypt'),
    ('Alexandria', 31.2001, 29.9187, 'Egypt'),
    ('Lagos', 6.5244, 3.3792, 'Nigeria'),
    ('Kinshasa', -4.4419, 15.2663, 'DR Congo'),
    ('Nairobi', -1.2921, 36.8219, 'Kenya'),
    ('Addis Ababa', 9.0300, 38.7400, 'Ethiopia'),
    ('Johannesburg', -26.2041, 28.0473, 'South Africa'),
    ('Cape Town', -33.9249, 18.4241, 'South Africa'),
    ('Casablanca', 33.5731, -7.5898, 'Morocco'),
    ('Algiers', 36.7538, 3.0588, 'Algeria'),
    ('Tunis', 36.8065, 10.1815, 'Tunisia'),
    ('Accra', 5.6037, -0.1870, 'Ghana'),
    ('Dakar', 14.7167, -17.4677, 'Senegal'),

    -- Oceania
    ('Sydney', -33.8688, 151.2093, 'Australia'),
    ('Melbourne', -37.8136, 144.9631, 'Australia'),
    ('Brisbane', -27.4698, 153.0251, 'Australia'),
    ('Perth', -31.9505, 115.8605, 'Australia'),
    ('Auckland', -36.8485, 174.7633, 'New Zealand'),
    ('Wellington', -41.2865, 174.7762, 'New Zealand');