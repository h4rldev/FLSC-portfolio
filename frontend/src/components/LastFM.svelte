<script>
	import { env } from '$env/dynamic/public';
	import { browser } from '$app/environment';
	import { languageTag } from '$lib/paraglide/runtime';

	import Icon from '@iconify/svelte';
	import * as m from '$lib/paraglide/messages.js';

	const api_key = '247b1a4b7dd5d6c42bb732941db23aef';
	const url = `https://ws.audioscrobbler.com/2.0/?method=user.getrecenttracks&user=h4rl3h&api_key=${api_key}&format=json`;

	let data = $state();
	let time = $state();

	async function fetchData() {
		if (browser) {
			const response = await fetch(url);
			const retrieved_data = await response.json();
			data = retrieved_data.recenttracks.track[0];

			let time_gmt = new Date(data.date['#text']);
			let time_offset = -time_gmt.getTimezoneOffset();
			const local_time = new Date(time_gmt.getTime() + time_offset * 60 * 1000).toLocaleTimeString(
				languageTag(),
				{
					hour: 'numeric',
					minute: 'numeric'
				}
			);

			time = local_time;
		}
	}
</script>

{#await fetchData()}
	<p>{m.loading()}</p>
{:then}
	<div class="text">
		{m.last_listened()}
		<a href="https://last.fm" target="_blank"
			><Icon icon="simple-icons:lastdotfm" class="w-[1em]" /></a
		>
	</div>
	<div class="lastfm-container">
		<div class="image">
			<a href={data.url} target="_blank">
				<img src={data.image[3]['#text']} class="image-thumbnail" alt={data.name} />
			</a>
		</div>
		<ul>
			<li><a href={data.url} target="_blank">{data.name}</a></li>
			<li>
				<a href="https://www.last.fm/music/{data.artist['#text']}" target="_blank"
					>{data.artist['#text']}</a
				>
			</li>
			<li>{data.album['#text']}</li>
			<li>
				Time of listening: {time}
			</li>
		</ul>
	</div>
{:catch error}
	<p style="text-red-500">{error.message}</p>
{/await}

<style>
	.text {
		@apply flex flex-row items-center gap-1 text-sm;
	}

	.image {
		@apply flex flex-col items-center justify-center p-0;
	}

	.image-thumbnail {
		@apply size-24 rounded-lg border-2 border-transparent hover:border-blue-400;
		@apply transition-colors duration-300 ease-in-out;
	}

	.lastfm-container {
		@apply flex flex-row gap-2 text-sm xl:text-xs 2xl:text-sm;
	}

	ul {
		@apply flex flex-col justify-center;
	}

	li {
		@apply inline xl:w-[90px] 2xl:w-fit;
	}
</style>
