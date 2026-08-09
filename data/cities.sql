CREATE TABLE IF NOT EXISTS cities (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    latitude REAL NOT NULL,
    longitude REAL NOT NULL,
    country TEXT NOT NULL,
    difficulty TEXT NOT NULL CHECK (difficulty IN ('easy', 'medium', 'hard'))
);

INSERT OR IGNORE INTO cities (name, latitude, longitude, country, difficulty) VALUES
    -- USA
    ('New York', 40.7128, -74.0060, 'USA', 'easy'),
    ('Los Angeles', 34.0522, -118.2437, 'USA', 'easy'),
    ('Chicago', 41.8781, -87.6298, 'USA', 'easy'),
    ('Houston', 29.7604, -95.3698, 'USA', 'medium'),
    ('Miami', 25.7617, -80.1918, 'USA', 'easy'),
    ('Seattle', 47.6062, -122.3321, 'USA', 'medium'),
    ('San Francisco', 37.7749, -122.4194, 'USA', 'easy'),
    ('Boston', 42.3601, -71.0589, 'USA', 'medium'),
    ('Washington', 38.9072, -77.0369, 'USA', 'easy'),
    ('Philadelphia', 39.9526, -75.1652, 'USA', 'medium'),
    ('Atlanta', 33.7490, -84.3880, 'USA', 'medium'),
    ('Denver', 39.7392, -104.9903, 'USA', 'medium'),
    ('Dallas', 32.7767, -96.7970, 'USA', 'medium'),
    ('Phoenix', 33.4484, -112.0740, 'USA', 'hard'),

    -- Russia
    ('Moscow', 55.7558, 37.6173, 'Russia', 'easy'),
    ('Saint Petersburg', 59.9343, 30.3351, 'Russia', 'easy'),
    ('Novosibirsk', 55.0084, 82.9357, 'Russia', 'hard'),
    ('Yekaterinburg', 56.8389, 60.6057, 'Russia', 'hard'),

    -- UK / Ireland
    ('London', 51.5074, -0.1278, 'UK', 'easy'),
    ('Birmingham', 52.4862, -1.8904, 'UK', 'medium'),
    ('Manchester', 53.4808, -2.2426, 'UK', 'medium'),
    ('Glasgow', 55.8642, -4.2518, 'UK', 'medium'),
    ('Edinburgh', 55.9533, -3.1883, 'UK', 'medium'),
    ('Dublin', 53.3498, -6.2603, 'Ireland', 'medium'),

    -- France
    ('Paris', 48.8566, 2.3522, 'France', 'easy'),
    ('Marseille', 43.2965, 5.3698, 'France', 'medium'),
    ('Lyon', 45.7640, 4.8357, 'France', 'medium'),
    ('Toulouse', 43.6047, 1.4442, 'France', 'hard'),

    -- Germany / Austria / Switzerland
    ('Berlin', 52.5200, 13.4050, 'Germany', 'easy'),
    ('Hamburg', 53.5511, 9.9937, 'Germany', 'medium'),
    ('Munich', 48.1351, 11.5820, 'Germany', 'easy'),
    ('Cologne', 50.9375, 6.9603, 'Germany', 'medium'),
    ('Vienna', 48.2082, 16.3738, 'Austria', 'easy'),
    ('Zurich', 47.3769, 8.5417, 'Switzerland', 'medium'),
    ('Geneva', 46.2044, 6.1432, 'Switzerland', 'medium'),

    -- Italy / Spain / Portugal
    ('Rome', 41.9028, 12.4964, 'Italy', 'easy'),
    ('Milan', 45.4642, 9.1900, 'Italy', 'easy'),
    ('Naples', 40.8518, 14.2681, 'Italy', 'medium'),
    ('Madrid', 40.4168, -3.7038, 'Spain', 'easy'),
    ('Barcelona', 41.3851, 2.1734, 'Spain', 'easy'),
    ('Valencia', 39.4699, -0.3763, 'Spain', 'medium'),
    ('Lisbon', 38.7223, -9.1393, 'Portugal', 'medium'),
    ('Porto', 41.1579, -8.6291, 'Portugal', 'hard'),

    -- Benelux
    ('Amsterdam', 52.3676, 4.9041, 'Netherlands', 'easy'),
    ('Rotterdam', 51.9244, 4.4777, 'Netherlands', 'medium'),
    ('Brussels', 50.8503, 4.3517, 'Belgium', 'medium'),
    ('Antwerp', 51.2194, 4.4025, 'Belgium', 'hard'),

    -- Northern / Eastern Europe
    ('Stockholm', 59.3293, 18.0686, 'Sweden', 'medium'),
    ('Gothenburg', 57.7089, 11.9746, 'Sweden', 'hard'),
    ('Oslo', 59.9139, 10.7522, 'Norway', 'medium'),
    ('Copenhagen', 55.6761, 12.5683, 'Denmark', 'medium'),
    ('Helsinki', 60.1699, 24.9384, 'Finland', 'medium'),
    ('Warsaw', 52.2297, 21.0122, 'Poland', 'medium'),
    ('Krakow', 50.0647, 19.9450, 'Poland', 'hard'),
    ('Prague', 50.0755, 14.4378, 'Czech Republic', 'easy'),
    ('Budapest', 47.4979, 19.0402, 'Hungary', 'medium'),
    ('Bucharest', 44.4268, 26.1025, 'Romania', 'hard'),
    ('Sofia', 42.6977, 23.3219, 'Bulgaria', 'hard'),
    ('Athens', 37.9838, 23.7275, 'Greece', 'easy'),
    ('Kyiv', 50.4501, 30.5234, 'Ukraine', 'medium'),

    -- Asia — East
    ('Tokyo', 35.6762, 139.6503, 'Japan', 'easy'),
    ('Osaka', 34.6937, 135.5023, 'Japan', 'medium'),
    ('Yokohama', 35.4437, 139.6380, 'Japan', 'hard'),
    ('Beijing', 39.9042, 116.4074, 'China', 'easy'),
    ('Shanghai', 31.2304, 121.4737, 'China', 'easy'),
    ('Guangzhou', 23.1291, 113.2644, 'China', 'hard'),
    ('Shenzhen', 22.5431, 114.0579, 'China', 'hard'),
    ('Chengdu', 30.5728, 104.0668, 'China', 'hard'),
    ('Wuhan', 30.5928, 114.3055, 'China', 'hard'),
    ('Xian', 34.3416, 108.9398, 'China', 'hard'),
    ('Seoul', 37.5665, 126.9780, 'South Korea', 'easy'),
    ('Busan', 35.1796, 129.0756, 'South Korea', 'medium'),

    -- Asia — South / Southeast
    ('Mumbai', 19.0760, 72.8777, 'India', 'easy'),
    ('Delhi', 28.7041, 77.1025, 'India', 'easy'),
    ('Bangalore', 12.9716, 77.5946, 'India', 'medium'),
    ('Kolkata', 22.5726, 88.3639, 'India', 'medium'),
    ('Chennai', 13.0827, 80.2707, 'India', 'hard'),
    ('Hyderabad', 17.3850, 78.4867, 'India', 'hard'),
    ('Jakarta', -6.2088, 106.8456, 'Indonesia', 'medium'),
    ('Surabaya', -7.2575, 112.7521, 'Indonesia', 'hard'),
    ('Manila', 14.5995, 120.9842, 'Philippines', 'medium'),
    ('Bangkok', 13.7563, 100.5018, 'Thailand', 'easy'),
    ('Hanoi', 21.0285, 105.8542, 'Vietnam', 'medium'),
    ('Ho Chi Minh City', 10.8231, 106.6297, 'Vietnam', 'medium'),
    ('Singapore', 1.3521, 103.8198, 'Singapore', 'easy'),
    ('Kuala Lumpur', 3.1390, 101.6869, 'Malaysia', 'medium'),
    ('Karachi', 24.8607, 67.0011, 'Pakistan', 'medium'),
    ('Lahore', 31.5497, 74.3436, 'Pakistan', 'hard'),
    ('Dhaka', 23.8103, 90.4125, 'Bangladesh', 'hard'),

    -- Middle East / Central Asia
    ('Istanbul', 41.0082, 28.9784, 'Turkey', 'easy'),
    ('Ankara', 39.9334, 32.8597, 'Turkey', 'hard'),
    ('Tehran', 35.6892, 51.3890, 'Iran', 'medium'),
    ('Riyadh', 24.7136, 46.6753, 'Saudi Arabia', 'medium'),
    ('Jeddah', 21.4858, 39.1925, 'Saudi Arabia', 'hard'),
    ('Dubai', 25.2048, 55.2708, 'UAE', 'easy'),
    ('Abu Dhabi', 24.4539, 54.3773, 'UAE', 'medium'),
    ('Tel Aviv', 32.0853, 34.7818, 'Israel', 'medium'),
    ('Jerusalem', 31.7683, 35.2137, 'Israel', 'easy'),
    ('Baghdad', 33.3152, 44.3661, 'Iraq', 'medium'),
    ('Almaty', 43.2220, 76.8512, 'Kazakhstan', 'hard'),
    ('Astana', 51.1694, 71.4491, 'Kazakhstan', 'hard'),
    ('Tashkent', 41.2995, 69.2401, 'Uzbekistan', 'hard'),
    ('Yerevan', 40.1792, 44.4991, 'Armenia', 'hard'),
    ('Tbilisi', 41.7151, 44.8271, 'Georgia', 'hard'),
    ('Baku', 40.4093, 49.8671, 'Azerbaijan', 'hard'),
    ('Dushanbe', 38.5598, 68.7870, 'Tajikistan', 'hard'),

    -- Canada / Mexico
    ('Toronto', 43.6511, -79.3470, 'Canada', 'easy'),
    ('Vancouver', 49.2827, -123.1207, 'Canada', 'medium'),
    ('Montreal', 45.5017, -73.5673, 'Canada', 'medium'),
    ('Mexico City', 19.4326, -99.1332, 'Mexico', 'easy'),
    ('Guadalajara', 20.6597, -103.3496, 'Mexico', 'hard'),

    -- South America
    ('Sao Paulo', -23.5505, -46.6333, 'Brazil', 'medium'),
    ('Rio de Janeiro', -22.9068, -43.1729, 'Brazil', 'easy'),
    ('Brasilia', -15.8267, -47.9218, 'Brazil', 'hard'),
    ('Buenos Aires', -34.6037, -58.3816, 'Argentina', 'easy'),
    ('Santiago', -33.4489, -70.6693, 'Chile', 'medium'),
    ('Lima', -12.0464, -77.0428, 'Peru', 'medium'),
    ('Bogota', 4.7110, -74.0721, 'Colombia', 'medium'),
    ('Caracas', 10.4806, -66.9036, 'Venezuela', 'hard'),
    ('Quito', -0.1807, -78.4678, 'Ecuador', 'hard'),
    ('Montevideo', -34.9011, -56.1645, 'Uruguay', 'hard'),

    -- Africa
    ('Cairo', 30.0444, 31.2357, 'Egypt', 'easy'),
    ('Alexandria', 31.2001, 29.9187, 'Egypt', 'medium'),
    ('Lagos', 6.5244, 3.3792, 'Nigeria', 'medium'),
    ('Kinshasa', -4.4419, 15.2663, 'DR Congo', 'hard'),
    ('Nairobi', -1.2921, 36.8219, 'Kenya', 'medium'),
    ('Addis Ababa', 9.0300, 38.7400, 'Ethiopia', 'hard'),
    ('Johannesburg', -26.2041, 28.0473, 'South Africa', 'medium'),
    ('Cape Town', -33.9249, 18.4241, 'South Africa', 'easy'),
    ('Casablanca', 33.5731, -7.5898, 'Morocco', 'medium'),
    ('Algiers', 36.7538, 3.0588, 'Algeria', 'hard'),
    ('Tunis', 36.8065, 10.1815, 'Tunisia', 'hard'),
    ('Accra', 5.6037, -0.1870, 'Ghana', 'hard'),
    ('Dakar', 14.7167, -17.4677, 'Senegal', 'hard'),

    -- Oceania
    ('Sydney', -33.8688, 151.2093, 'Australia', 'easy'),
    ('Melbourne', -37.8136, 144.9631, 'Australia', 'medium'),
    ('Brisbane', -27.4698, 153.0251, 'Australia', 'hard'),
    ('Perth', -31.9505, 115.8605, 'Australia', 'hard'),
    ('Auckland', -36.8485, 174.7633, 'New Zealand', 'medium'),
    ('Wellington', -41.2865, 174.7762, 'New Zealand', 'hard');