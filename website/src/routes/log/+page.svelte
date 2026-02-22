<script lang="ts">
	import type { PageProps } from "./$types"

	let props: PageProps = $props()
</script>

<div class="m-2 flex flex-col gap-2">
	{#each props.data.logs as log}
		{@const parsedTimestamp = new Date(log.timestamp)}

		<div class="card preset-tonal w-full p-2">
			<small class="opacity-50">{parsedTimestamp.toLocaleTimeString("en-US")}</small>
			<p>{log.text}</p>
		</div>
	{/each}
</div>
