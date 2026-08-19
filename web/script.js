let map = null;
let start_marker = false;
let markers = []; 
let session_id = null;
let start_city_name = '';
let attempts_left = 10;
let current_distance = 0;
let guessed_counter = 0;
let score = 0; 
let citySuggestions = []; 
let start_button = 'start-game-button'; 

function init_map() {
    map = L.map('map').setView([20, 0], 2); 
    L.tileLayer('https://{s}.basemaps.cartocdn.com/light_nolabels/{z}/{x}/{y}{r}.png', {
        attribution: '&copy; OpenStreetMap contributors',
    }).addTo(map);
}

function delete_markers() {
    markers.forEach(marker => map.removeLayer(marker));
    markers = [];
    start_marker = false;
}

function add_city_marker(city, angle = 0, distance = 0) {
    if (start_marker == true) {
        delete_markers();
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

function update_pips() {
    const pips = document.querySelectorAll('#pips .pip');
    pips.forEach((pip, index) => {
        if (index < attempts_left) {
            pip.classList.add('on');
        } else {
            pip.classList.remove('on');
        }
    });
}

function update_UI() {
    document.getElementById('attempts-number').textContent = attempts_left;
    const msg_el = document.getElementById('message');
    msg_el.textContent = 'Choose settings and press «Start game»';
    msg_el.className = 'message';
    update_pips();
}

function new_game_UI() {
    document.getElementById('start-city').textContent = '—';
    document.getElementById('attempts-number').textContent = 10;
    document.getElementById('guessed-number').textContent = 0;
    document.getElementById('input-field').disabled = true;
    document.getElementById('check-button').disabled = true;
    document.getElementById('vec-dir').textContent = '— —';
    document.getElementById('vec-dist').textContent = 'Distance: —';
    const msg_el = document.getElementById('message');
    msg_el.textContent = 'Choose settings and press «Start game»';
    msg_el.className = 'message';
    attempts_left = 10;
    update_pips();
    set_difficulty_locked(false);
    set_capitals_only_locked(false);
    delete_markers(); 
}

function set_difficulty_locked(locked) {
    const selected = document.getElementById('difficulty-select');
    if (locked) {
        selected.classList.add('locked');
    } else {
        selected.classList.remove('locked');
    }
}

function get_selected_difficulty() {
    const activeOption = document.querySelector('#difficulty-select .difficulty-option.active');
    return activeOption ? activeOption.getAttribute('data-value') : 'easy'; 
}

function set_capitals_only_locked(locked) {
    document.getElementById('capitals-only-checkbox').disabled = locked;
}

function get_capitals_only() {
    return document.getElementById('capitals-only-checkbox').checked;
}

function show_gameplay_controls() {
    document.getElementById('input-field').disabled = false;
    document.getElementById('input-field').value = '';
    document.getElementById('check-button').style.display = '';
}

function show_game_over(text) {
    const msg_el = document.getElementById('message');
    msg_el.textContent = text;
    msg_el.className = 'message-error';
    document.getElementById('input-field').disabled = true;
    document.getElementById('check-button').disabled = true;
    const restart_btn = document.getElementById('start-game-button');
    restart_btn.textContent = 'New Game';
    restart_btn.style.display = 'inline-block';
    set_difficulty_locked(false);
    set_capitals_only_locked(false);
}

async function load_start_city() {
    try {
        const difficulty = get_selected_difficulty();
        const capitals_only = get_capitals_only();
        console.log(`Loading start city with difficulty=${difficulty}, capitals_only=${capitals_only}`);
        const resp = await fetch(`/api/start?difficulty=${encodeURIComponent(difficulty)}&capitals_only=${encodeURIComponent(capitals_only)}`);
        if (!resp.ok) throw new Error(`HTTP ${resp.status}`);
        const data = await resp.json();
        start_city_name = data.city.name;
        session_id = data.session_id;
        attempts_left = data.attempts_left ?? 5;
        current_distance = data.vector.distance_km;
        guessed_counter = 0;
        score = 0;
        document.getElementById('start-city').textContent = start_city_name;
        let deg = parseFloat(data.vector.angle_deg);
        document.getElementById('vec-dir').textContent = `${deg.toFixed(0)}°`;
        const el = document.getElementById('compass-circle').style.setProperty('--deg', `${Math.round(90 + deg)}deg`);

        document.getElementById('vec-dist').textContent = `Distance: ${parseFloat(data.vector.distance_km).toFixed(0)} km`;
        document.getElementById('guessed-number').textContent = 0;
        document.getElementById('score').textContent = 'Score: 0';
        start_marker = true; 
        add_city_marker(data.city, data.vector.angle_deg, data.vector.distance_km);
        update_UI();
        document.getElementById('check-button').disabled = false;
        show_gameplay_controls();
        document.getElementById('message').textContent = `Where does the vector from ${start_city_name} lead?`;
        document.getElementById('message').className = 'message';
        set_difficulty_locked(true);
        set_capitals_only_locked(true);
        await fetch_city_suggestions(difficulty, capitals_only);
    } catch (err) {
        console.error('Failed to load start city:', err);
        document.getElementById('start-city').textContent = 'Error';
        document.getElementById('vec-dir').textContent = 'Error';
        document.getElementById('vec-dist').textContent = 'Error';
    }
}

async function fetch_city_suggestions(difficulty, capitals_only) {
    try {
        const resp = await fetch(`/api/cities?difficulty=${encodeURIComponent(difficulty)}&capitals_only=${encodeURIComponent(capitals_only)}`);
        if (!resp.ok) throw new Error(`HTTP ${resp.status}`);
        const data = await resp.json();
        citySuggestions = Object.values(data);
        console.log(`Fetched ${citySuggestions.length} city suggestions for difficulty=${difficulty}, capitals_only=${capitals_only}`);
    } catch (err) {
        console.error('Failed to fetch city suggestions:', err);
        citySuggestions = [];
    }
}

async function load_next_round() {
    try {
        const resp = await fetch(`/api/next_round?session_id=${encodeURIComponent(session_id)}`);
        if (!resp.ok) throw new Error(`HTTP ${resp.status}`);
        const data = await resp.json();
        start_city_name = data.city.name;
        attempts_left = data.attempts_left ?? 10;
        document.getElementById('start-city').textContent = start_city_name;
        let deg = parseFloat(data.vector.angle_deg);
        document.getElementById('vec-dir').textContent = `${deg.toFixed(0)}°`;
        const el = document.getElementById('compass-circle').style.setProperty('--deg', `${Math.round(90 + deg)}deg`);
        document.getElementById('vec-dist').textContent = `Distance: ${parseFloat(data.vector.distance_km).toFixed(0)} km`;
        add_city_marker(data.city, data.vector.angle_deg, data.vector.distance_km);
        update_UI();
        document.getElementById('input-field').value = '';
        document.getElementById('input-field').disabled = false;
        document.getElementById('check-button').disabled = false;
        show_gameplay_controls();
        const msg_el = document.getElementById('message');
        msg_el.textContent = 'Correct!';
        msg_el.className = 'message-correct';
    } catch (err) {
        console.error('Failed to load next round:', err);
        document.getElementById('message').textContent = 'Error loading next round';
        document.getElementById('message').className = 'message-error';
    }
}

async function make_guess() {
    const guess_input = document.getElementById('input-field');
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
            msg_el.className = 'message-correct';
            guess_input.value = '';
            start_marker = true; 
            document.getElementById('guessed-number').textContent = ++guessed_counter;
            score += current_distance;
            document.getElementById('score').textContent = `Score: ${score.toFixed(0)}`;
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
        msg_el.className = 'message-error';
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
        document.getElementById('message').className = 'message-error';
    }
}

const dropdown = document.getElementById('dropdown-list');  
const input = document.getElementById('input-field');
input.addEventListener('input', () => {
    const query = input.value.toLowerCase();
    dropdown.innerHTML = ''; 
    if (!query) {
        dropdown.classList.add('hidden');
        return;
    }
    const filtered = citySuggestions.filter(item => item.toLowerCase().includes(query));

    if (filtered.length > 0) {
        filtered.forEach(item => {
            const div = document.createElement('div');
            div.classList.add('dropdown-item');
            div.textContent = item;
            div.addEventListener('click', () => {
                input.value = item;
                dropdown.classList.add('hidden');
            });
            dropdown.appendChild(div);
        });
        dropdown.classList.remove('hidden');
    } else {
        dropdown.classList.add('hidden');
    }
});

document.querySelectorAll('#difficulty-select .difficulty-option').forEach(option => {
    option.addEventListener('click', function() {
        if (document.getElementById('difficulty-select').classList.contains('locked')) {
            return; 
        }
        document.querySelector('#difficulty-select .difficulty-option.active').classList.remove('active');
        this.classList.add('active');
    });
});

document.getElementById('check-button').addEventListener('click', make_guess);
document.getElementById('input-field').addEventListener('keypress', e => {
    if (e.key === 'Enter') make_guess();
    if (!e.target.closest('.autocomplete-container')) {
        dropdown.classList.add('hidden');
    }
});

document.addEventListener('click', (e) => {
    if (!e.target.closest('#input-field') && !e.target.closest('#dropdown-list')) {
        dropdown.classList.add('hidden');
    }
});

document.getElementById(start_button).addEventListener('click', () => {
    const element = document.getElementById(start_button); 
    console.log(element);
    if (element.id == 'start-game-button') {
        element.textContent = 'New Game'; 
        element.id = 'new-game-button'; 
        start_button = element.id; 
        load_start_city();
    }
    else {
        element.textContent = 'Stat Game'; 
        element.id = 'start-game-button'; 
        start_button = element.id; 
        new_game_UI();
    }
});

window.addEventListener('DOMContentLoaded', () => {
    document.getElementById('input-field').value = '';
    document.getElementById('input-field').disabled = true;
    document.getElementById('check-button').disabled = true;
    init_map();
});