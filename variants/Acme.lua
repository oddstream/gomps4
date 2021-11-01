-- Acme

--[[
  https://en.wikipedia.org/wiki/Acme_(solitaire)

  Acme is a Canfield type of patience or solitaire card game using a single deck of playing cards.
  Acme has four tableau stacks of one card each, and they are built down in suit.
  There are also four Foundations that build up in suit.
  The Reserve Pile contains 13 cards which can be played onto the Foundations or Tableau Stacks.
  The deck turns up one card at a time.
  Only the top card of a Tableau stack can be moved.
  These cards can be moved to a Foundation or onto another Tableau stack.
  The Tableau builds down in suit, and the Foundations build up in suit.
  Cards from the Reserve automatically fill empty spaces.
  Any card can fill empty Tableau spaces after the Reserve is empty.
  There is only one redeal allowed in this game, so only two passes through the deck are allowed.
  Strategy: Rather than using the cards from the deck, a player should try to use all of the reserve cards first.
  Only two passes are allowed, so the deck should be used wisely.
]]

dofile("variants/~Library.lua")

POWER_MOVES = false
SEED=12620
STOCK_DEAL_CARDS = 1

function BuildPiles()

    STOCK = AddPile("Stock", 1, 1, FAN_NONE, 1, 4)
    WASTE = AddPile("Waste", 2, 1, FAN_RIGHT3)
    
    local pile

    FOUNDATIONS = {}
    for x = 4, 7 do
        pile = AddPile("Foundation", x, 1, FAN_NONE)
        PileAccept(pile, 1)
        table.insert(FOUNDATIONS, pile)
    end

    RESERVES = {}
    pile = AddPile("Reserve", 1, 2, FAN_DOWN)
    table.insert(RESERVES, pile)
    for n = 1, 13 do
        local c = MoveCard(STOCK, pile)
        CardProne(c, true)
    end
    CardProne(Last(pile), false)

    TABLEAUX = {}
    for x = 4, 7 do
        pile = AddPile("Tableau", x, 2, FAN_DOWN)
        table.insert(TABLEAUX, pile)
        MoveCard(STOCK, pile)
    end

end

function StartGame()
    STOCK_RECYCLES = 1
end

-- TailMoveError constraints (Tableau only)

function Tableau.TailMoveError(tail)
    local c1 = Get(tail, 1)
    if POWER_MOVES then
        for i = 2, Len(tail) do
            local c2 = Get(tail, i)
            local err = DownSuit(c1, c2) if err then return err end
            c1 = c2
        end
    else
        if Len(tail) > 1 then
            return "Can only move a single card"
        end
    end
    return nil
end

-- TailAppendError constraints

function Waste.TailAppendError(pile, tail)
    if CardOwner(First(tail)) ~= STOCK then
        return "The Waste can only accept cards from the Stock"
    end
    return nil
end

function Foundation.TailAppendError(pile, tail)
    if Len(pile) == 0 then
        local c1 = First(tail)
        if CardOrdinal(c1) ~= 1 then
            return "Foundation can only accept an Ace, not a " .. V[CardOrdinal(c1)]
        end
    else
        local c1 = Last(pile)
        local c2 = First(tail)
        local err = UpSuit(c1, c2) if err then return err end
    end
    return nil
end

function Tableau.TailAppendError(pile, tail)
    if Len(pile) == 0 then
        -- do nothing, empty accept any card
    else
        local c1 = Last(pile)
        local c2 = First(tail)
        local err = DownSuit(c1, c2) if err then return err end
    end
    return nil
end

-- PileConformantError

function Tableau.PileConformantError(pile)
    local c1 = First(pile)
    for i = 2, Len(pile) do
        local c2 = Get(pile, n)
        local err = DownSuit(c1, c2) if err then return err end
        c1 = c2
    end
    return nil
end

-- SortedAndUnSorted (Tableau only)

function Tableau.SortedAndUnsorted(pile)
    local sorted = 0
    local unsorted = 0
    local c1 = Get(pile, 1)
    for i = 2, Len(pile) do
        local c2 = Get(pile, i)
        local err = DownSuit(c1, c2)
        if err then
            unsorted = unsorted + 1
        else
            sorted = sorted + 1
        end
        c1 = c2
    end
    return sorted, unsorted
end

-- Actions

function Stock.Tapped(tail)
    if tail == nil then
        if STOCK_RECYCLES == 0 then
            Toast("No more Stock recycles")
            return
        end
        if Len(WASTE) > 0 then
            while Len(WASTE) > 0 do
                MoveCard(WASTE, STOCK)
            end
            STOCK_RECYCLES = STOCK_RECYCLES - 1
        end
    else
        for i = 1, STOCK_DEAL_CARDS do
            MoveCard(STOCK, WASTE)
        end
    end
end

function AfterMove()
    for i = 1, 4 do
        if Len(TABLEAUX[i]) == 0 then
            MoveCard(RESERVES[1], TABLEAUX[i])
        end
    end
end
