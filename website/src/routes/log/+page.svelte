<script lang="ts">
	import type { PageProps } from "./$types"

	let props: PageProps = $props()
	const logs = $derived(
		props.data.logs.toSorted((a, b) => {
			const dateA = Date.parse(a.timestamp)
			const dateB = Date.parse(b.timestamp)

			return dateB - dateA
		})
	)
</script>

<div class="m-2 flex flex-col gap-2">
	{#each logs as log}
		{@const parsedTimestamp = new Date(log.timestamp)}

		<div class="card preset-tonal w-full p-2">
			<small class="opacity-50"
				>{parsedTimestamp.toLocaleDateString("vi-VN")}
				{parsedTimestamp.toLocaleTimeString("vi-VN")}</small
			>
			<p>{log.text}</p>
		</div>
	{/each}
</div>
