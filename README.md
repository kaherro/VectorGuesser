# VectorGuesser (http://vectorguesser.duckdns.org or http://vectorguesser.chickenkiller.com/)

![Demonstation](demo.gif)

A geography-based guessing game where players use vectors (distance and bearing) to guess hidden cities. Each correct guess increases your score by distance to the target, while incorrect guesses decrease attempts. The goal is to guess as many cities as possible and get as much score as you can. 

## Features

- **Dynamic Difficulty Levels**: Easy, Medium, and Hard city selections
- **Capitals Only Mode**: Toggle to guess only capital cities
- **Vector-Based Gameplay**: Each round shows a start city, distance, and bearing to a hidden target city
- **Interactive Map**: Visualized vectors and city locations using Leaflet.js
- **Autocomplete Suggestions**: Dynamic city suggestions based on selected difficulty and mode
- **Scoring System**: Earn points based on distance of correct guesses
- **Session Management**: Track attempts, cities guessed, and score
- **RESTful API**: C++ backend with Crow framework
- **SQLite Database**: Pre-populated with world cities and their attributes
- **Docker Support**: Easy deployment with containerization

## Technical Stack

### Backend
- **Language**: C++20
- **HTTP Server**: Crow (header-only)
- **Database**: SQLite3 with RAII wrappers
- **Build System**: CMake 3.16+
- **Containerization**: Docker

### Frontend
- **Map Library**: Leaflet.js
- **Styling**: CSS3
- **Logic**: JavaScript
- **Assets**: Custom vector markers

### Database Schema
```sql
CREATE TABLE cities (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    latitude REAL NOT NULL,
    longitude REAL NOT NULL,
    country TEXT NOT NULL,
    difficulty TEXT NOT NULL CHECK (difficulty IN ('easy', 'medium', 'hard')),
    capital INTEGER NOT NULL DEFAULT 0 CHECK (capital IN (0, 1))
);
```

## Setup Instructions

### Prerequisites
- C++ compiler with C++20 support (GCC 9+, Clang 9+, MSVC 2019+)
- CMake 3.16 or higher
- SQLite3 development libraries
- Git
- Docker (optional, for containerized deployment)

### Docker Deployment

1. **Build the Docker image**
   ```bash
   docker build -t vectorguesser .
   ```

2. **Run the container**
   ```bash
   docker run -p 80:18080 vectorguesser
   ```

The server will start on `http://0.0.0.0`

### Local Development

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd VectorGuesser
   ```

2. **Build the project**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. **Initialize the database** (if not already present)
   ```bash
   sqlite3 data/cities.db < data/cities.sql
   ```

4. **Run the server**
   ```bash
   ./VectorGuesser
   ```

## API Endpoints

### GET `/`
- Redirects to `/index.html`

### GET `/<string>`
- Serves static files from the `web` directory

### GET `/api/start`
- Starts a new game session
- **Query Parameters**:
  - `difficulty` (required): `easy`, `medium`, or `hard`
  - `capitals_only` (required): `true` or `false`
- **Response**:
  ```json
  {
    "city": {
      "id": <integer>,
      "name": <string>,
      "latitude": <number>,
      "longitude": <number>,
      "country": <string>
    },
    "session_id": <string>,
    "attempts_left": <integer>,
    "vector": {
      "distance_km": <integer>,
      "angle_deg": <number>
    }
  }
  ```

### GET `/api/guess`
- Makes a guess for the current session
- **Query Parameters**:
  - `from` (required): City name to guess
  - `session_id` (required): Session identifier
- **Response**:
  - On correct guess: `"Correct"` (plain text)
  - On error with attempts left: JSON with city, vector, and attempts_left
  - On game over: `"No attempts left. Game over. Target city was: <city name>"` (plain text)
  - On error: JSON with error message

### GET `/api/next_round`
- Loads the next round in the same session
- **Query Parameters**:
  - `session_id` (required): Session identifier
- **Response**: Same format as `/api/start`

### GET `/api/cities`
- Gets list of city names for autocomplete
- **Query Parameters**:
  - `difficulty` (required): `easy`, `medium`, or `hard`
  - `capitals_only` (required): `true` or `false`
- **Response**: JSON array of city names

## File Structure

```
VectorGuesser/
├── data/
│   ├── cities.db          # SQLite database (generated from cities.sql)
│   └── cities.sql         # Database schema and initial data
├── src/
│   ├── main.cpp           # Entry point and API handlers
│   ├── db_manager.h       # Database manager declaration
│   └── db_manager.cpp     # Database manager implementation
├── web/
│   ├── index.html         # Main HTML file
│   ├── style.css          # Stylesheet
│   └── script.js          # Frontend game logic
├── CMakeLists.txt         # CMake build configuration
├── Dockerfile             # Docker build instructions
└── README.md              # This file
```

## How to Play

1. Select a difficulty level (Easy, Medium, Hard)
2. Optionally enable "Capitals Only" mode
3. Click "Start Game" to begin
4. A start city will be shown with a vector (distance and bearing) to a hidden target city
5. Type your guess in the input field and click "Guess" or press Enter
6. Correct guesses:
   - Increase your score by the distance
   - Load a new round with the same attempts
7. Incorrect guesses:
   - Consume one attempt
   - Show the error vector
8. Game ends when you run out of attempts
9. Click "New Game" to restart

## Acknowledgments

- Leaflet.js for interactive maps
- Crow C++ framework for the web server
- SQLite for embedded database
- OpenStreetMap contributors for map data