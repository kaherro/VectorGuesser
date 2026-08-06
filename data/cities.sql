CREATE TABLE IF NOT EXISTS cities (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    latitude REAL NOT NULL,
    longitude REAL NOT NULL,
    country TEXT NOT NULL
);

INSERT OR IGNORE INTO cities (name, latitude, longitude, country) VALUES
    ('New York', 40.7128, -74.0060, 'USA'),
    ('Los Angeles', 34.0522, -118.2437, 'USA'),
    ('Chicago', 41.8781, -87.6298, 'USA'),
    ('Houston', 29.7604, -95.3698, 'USA'),
    ('Moscow', 55.7558, 37.6173, 'Russia'),
    ('Saint Petersburg', 59.9343, 30.3351, 'Russia'),
    ('Novosibirsk', 55.0084, 82.9357, 'Russia'),
    ('Yekaterinburg', 56.8389, 60.6057, 'Russia'),
    ('London', 51.5074, -0.1278, 'UK'),
    ('Birmingham', 52.4862, -1.8904, 'UK'),
    ('Manchester', 53.4808, -2.2426, 'UK'),
    ('Glasgow', 55.8642, -4.2518, 'UK'),
    ('Paris', 48.8566, 2.3522, 'France'),
    ('Marseille', 43.2965, 5.3698, 'France'),
    ('Lyon', 45.7640, 4.8357, 'France'),
    ('Toulouse', 43.6047, 1.4442, 'France'),
    ('Berlin', 52.5200, 13.4050, 'Germany'),
    ('Hamburg', 53.5511, 9.9937, 'Germany'),
    ('Munich', 48.1351, 11.5820, 'Germany'),
    ('Cologne', 50.9375, 6.9603, 'Germany');