let map = null;
let start_marker = false;
let markers = []; 
let session_id = null;
let start_city_name = '';
let attempts_left = 10;
let current_distance = 0;
let guessed_counter = 0;
let score = 0; 

function init_map() {
    map = L.map('map').setView([20, 0], 2); 
    L.tileLayer('https://{s}.basemaps.cartocdn.com/light_nolabels/{z}/{x}/{y}{r}.png', {
        attribution: '&copy; OpenStreetMap contributors',
    }).addTo(map);
}

function add_city_marker(city, angle = 0, distance = 0) {
    if (start_marker == true) {
        markers.forEach(marker => map.removeLayer(marker));
        markers = [];
        start_marker = false;
    }
    const icon_url = 'arrow-thin-up.png';
    const width = 15;
    const height = 31;

    const custom_icon = L.divIcon({
        className: 'rotated-marker-container',
        iconSize: [width, height],
        iconAnchor: [width / 2, 31],
        tooltipAnchor: [0, -height],
        html: `<img src="${icon_url}" style="width: ${width}px; height: ${height}px; transform: rotate(${angle}deg); transform-origin: bottom center;" />`
    });

    const marker = L.marker([parseFloat(city.latitude), parseFloat(city.longitude)], { icon: custom_icon })
        .addTo(map)
        .bindTooltip(city.name + '<br>' + distance + ' km', {permanent: true, direction: 'top'})
        .openTooltip();
    map.setView([parseFloat(city.latitude), parseFloat(city.longitude)], 4);
    markers.push(marker); 
    return marker;
}

function update_UI() {
    document.getElementById('attempts').textContent = attempts_left;
    const msg_el = document.getElementById('message');
    msg_el.textContent = '';
    msg_el.className = '';
}

function set_difficulty_locked(locked) {
    document.getElementById('difficulty-select').disabled = locked;
}

function get_selected_difficulty() {
    return document.getElementById('difficulty-select').value;
}

function show_gameplay_controls() {
    document.getElementById('guess-input').style.display = '';
    document.getElementById('guess-btn').style.display = '';
    document.getElementById('new-round-btn').style.display = 'none';
}

function show_game_over(text) {
    const msg_el = document.getElementById('message');
    msg_el.textContent = text;
    msg_el.className = 'error';
    document.getElementById('guess-input').disabled = true;
    document.getElementById('guess-btn').disabled = true;
    const restart_btn = document.getElementById('new-round-btn');
    restart_btn.textContent = 'New Game';
    restart_btn.style.display = 'inline-block';
    set_difficulty_locked(false);
}

async function load_start_city() {
    try {
        const difficulty = get_selected_difficulty();
        const resp = await fetch(`/api/start?difficulty=${encodeURIComponent(difficulty)}`);
        if (!resp.ok) throw new Error(`HTTP ${resp.status}`);
        const data = await resp.json();
        start_city_name = data.city.name;
        session_id = data.session_id;
        attempts_left = data.attempts_left ?? 5;
        current_distance = data.vector.distance_km;
        guessed_counter = 0;
        score = 0;
        document.getElementById('start-city').textContent = start_city_name;
        document.getElementById('vector-info').textContent =
            `${parseFloat(data.vector.distance_km).toFixed(0)} km @ ${parseFloat(data.vector.angle_deg).toFixed(0)}°`;
        document.getElementById('cities-guessed').textContent = 0;
        document.getElementById('score').textContent = 0;
        start_marker = true; 
        add_city_marker(data.city, data.vector.angle_deg, data.vector.distance_km);
        update_UI();
        document.getElementById('guess-input').value = '';
        document.getElementById('guess-input').disabled = false;
        document.getElementById('guess-btn').disabled = false;
        show_gameplay_controls();
        document.getElementById('message').textContent = '';
        set_difficulty_locked(true);
    } catch (err) {
        console.error('Failed to load start city:', err);
        document.getElementById('start-city').textContent = 'Error';
        document.getElementById('vector-info').textContent = 'Error';
    }
}

async function load_next_round() {
    try {
        const resp = await fetch(`/api/next_round?session_id=${encodeURIComponent(session_id)}`);
        if (!resp.ok) throw new Error(`HTTP ${resp.status}`);
        const data = await resp.json();
        start_city_name = data.city.name;
        attempts_left = data.attempts_left ?? 5;
        document.getElementById('start-city').textContent = start_city_name;
        document.getElementById('vector-info').textContent =
            `${parseFloat(data.vector.distance_km).toFixed(1)} km @ ${parseFloat(data.vector.angle_deg).toFixed(0)}°`;
        add_city_marker(data.city, data.vector.angle_deg, data.vector.distance_km);
        update_UI();
        document.getElementById('guess-input').value = '';
        document.getElementById('guess-input').disabled = false;
        document.getElementById('guess-btn').disabled = false;
        show_gameplay_controls();
        const msg_el = document.getElementById('message');
        msg_el.textContent = 'Correct!';
        msg_el.className = 'correct';
    } catch (err) {
        console.error('Failed to load next round:', err);
        document.getElementById('message').textContent = 'Error loading next round';
        document.getElementById('message').className = 'error';
    }
}

async function make_guess() {
    const guess_input = document.getElementById('guess-input');
    const guess = guess_input.value.trim();
    if (!guess) {
        alert('Please enter a city name');
        return;
    }
    try {
        const resp = await fetch(`/api/guess?from=${encodeURIComponent(guess)}&session_id=${encodeURIComponent(session_id)}`);
        if (!resp.ok) throw new Error(`HTTP ${resp.status}`);
        const raw_text = await resp.text();

        if (raw_text === 'Correct') {
            const msg_el = document.getElementById('message');
            msg_el.textContent = 'Correct!';
            msg_el.className = 'correct';
            guess_input.value = '';
            start_marker = true; 
            document.getElementById('cities-guessed').textContent = ++guessed_counter;
            score += current_distance;
            document.getElementById('score').textContent = score;
            await load_next_round();
            return;
        }

        if (raw_text.startsWith('No attempts left. Game over.')) {
            attempts_left = 0;
            update_UI();
            show_game_over(raw_text);
            guess_input.value = '';
            return;
        }

        let data;
        try {
            data = JSON.parse(raw_text);
        } catch (parseErr) {
            throw new Error(`Unexpected response: ${raw_text}`);
        }

        add_city_marker(data.city, data.vector.angle_deg, data.vector.distance_km);
        attempts_left = data.attempts_left ?? attempts_left;
        current_distance = data.vector.distance_km;
        update_UI();
        const msg_el = document.getElementById('message');
        msg_el.textContent = `Wrong. ${parseFloat(data.vector.distance_km).toFixed(0)} km @ ${parseFloat(data.vector.angle_deg).toFixed(0)}°`;
        msg_el.className = 'error';
        if (attempts_left <= 0) {
            show_game_over(msg_el.textContent + ' No attempts left. Game over.');
        }
        guess_input.value = '';
    } catch (err) {
        console.error('Guess failed:', err);
        if(err == 'Error: HTTP 404') {
            document.getElementById('message').textContent = 'City not found';
        }
        else {
            document.getElementById('message').textContent = 'Error contacting server';
        }
        document.getElementById('message').className = 'error';
    }
}

document.getElementById('guess-btn').addEventListener('click', make_guess);
document.getElementById('guess-input').addEventListener('keypress', e => {
    if (e.key === 'Enter') make_guess();
});

document.getElementById('new-round-btn').addEventListener('click', () => {
    document.getElementById('message').textContent = '';
    document.getElementById('message').className = '';
    load_start_city();
});

window.addEventListener('DOMContentLoaded', () => {
    init_map();
});